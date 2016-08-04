// **************************************************************************************
// User-defined memory manager, which maintains a pool of transactions
// **************************************************************************************
#ifndef Initiator_H
#define Initiator_H
#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;
using namespace tlm_utils;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"
#include "MM.h"
#include "Util.h"
// **************************************************************************************
// Initiator module generating multiple pipelined generic payload transactions
// **************************************************************************************

static ofstream fout("foo.txt");

struct Initiator: sc_module {
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    simple_initiator_socket<Initiator> socket;

    MM   m_mm;
    int  data[16];
    tlm_generic_payload* request_in_progress;
    sc_event end_request_event;
    peq_with_cb_and_phase<Initiator> m_peq;

    SC_CTOR(Initiator) :
        socket("socket"),
        request_in_progress(0),
        m_peq(this, &Initiator::peq_cb)
    {
        // Register callbacks for incoming interface method calls
        socket.register_nb_transport_bw(this, &Initiator::nb_transport_bw);
        SC_THREAD(thread_process);
    }

    void thread_process() {
        tlm_generic_payload* trans;
        tlm_phase phase;
        sc_time delay;

        // Generate a sequence of random transactions
        for (int i = 0; i < 1000; i++) {
            int adr = rand();
            tlm_command cmd = static_cast<tlm_command>(rand() % 2);
            if (cmd == TLM_WRITE_COMMAND) {
                data[i % 16] = rand();
            }

            // Grab a new transaction from the memory manager
            trans = m_mm.allocate();
            trans->acquire();

            // Set all attributes except byte_enable_length and extensions (unused)
            trans->set_command( cmd );
            trans->set_address( adr );
            trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data[i % 16]) );
            trans->set_data_length( 4 );
            trans->set_streaming_width( 4 ); // = data_length to indicate no streaming
            trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
            trans->set_dmi_allowed( false ); // Mandatory initial value
            trans->set_response_status( TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

            // Initiator must honor BEGIN_REQ/END_REQ exclusion rule
            if (request_in_progress) {
                wait(end_request_event);
            }
            request_in_progress = trans;
            phase = BEGIN_REQ;

            // Timing annotation models processing time of initiator prior to call
            delay = sc_time(rand_ps(), SC_PS);

            fout << hex << adr << " new, cmd=" << (cmd ? 'W' : 'R') << ", data=" << hex << data[i % 16] << " at time " << sc_time_stamp() << endl;

            // Non-blocking transport call on the forward path
            tlm_sync_enum status;
            status = socket->nb_transport_fw( *trans, phase, delay );

            // Check value returned from nb_transport_fw
            if (status == TLM_UPDATED) {
                // The timing annotation must be honored
                m_peq.notify( *trans, phase, delay );
            }
            else if (status == TLM_COMPLETED) {
                // The completion of the transaction necessarily ends the BEGIN_REQ phase
                request_in_progress = 0;

                // The target has terminated the transaction
                check_transaction( *trans );
            }
            wait( sc_time(rand_ps(), SC_PS) );
        }

        wait(100, SC_NS);

        // Allocate a transaction for one final, nominal call to b_transport
        trans = m_mm.allocate();
        trans->acquire();
        trans->set_command( TLM_WRITE_COMMAND );
        trans->set_address( 0 );
        trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data[0]) );
        trans->set_data_length( 4 );
        trans->set_streaming_width( 4 ); // = data_length to indicate no streaming
        trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
        trans->set_dmi_allowed( false ); // Mandatory initial value
        trans->set_response_status( TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

        delay = sc_time(rand_ps(), SC_PS);

        fout << "Calling b_transport at " << sc_time_stamp() << " with delay = " << delay << endl;

        // Call b_transport to demonstrate the b/nb conversion by the simple_target_socket
        socket->b_transport( *trans, delay );
        check_transaction( *trans );
    }

    // TLM-2 backward non-blocking transport method

    virtual tlm_sync_enum nb_transport_bw( tlm_generic_payload& trans, tlm_phase& phase, sc_time& delay ) {
        // The timing annotation must be honored
        m_peq.notify( trans, phase, delay );
        return TLM_ACCEPTED;
    }

    // Payload event queue callback to handle transactions from target
    // Transaction could have arrived through return path or backward path

    void peq_cb(tlm_generic_payload& trans, const tlm_phase& phase) {
        #ifdef DEBUG
            if (phase == END_REQ) {
                fout << hex << trans.get_address() << " END_REQ at " << sc_time_stamp() << endl;
            }
            else if (phase == BEGIN_RESP) {
                fout << hex << trans.get_address() << " BEGIN_RESP at " << sc_time_stamp() << endl;
            }
        #endif

        if (phase == END_REQ || (&trans == request_in_progress && phase == BEGIN_RESP)) {
            // The end of the BEGIN_REQ phase
            request_in_progress = 0;
            end_request_event.notify();
        }
        else if (phase == BEGIN_REQ || phase == END_RESP) {
            SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by initiator");
        }

        if (phase == BEGIN_RESP) {
            check_transaction( trans );

            // Send final phase transition to target
            tlm_phase fw_phase = END_RESP;
            sc_time delay = sc_time(rand_ps(), SC_PS);
            socket->nb_transport_fw( trans, fw_phase, delay );
            // Ignore return value
        }
    }

    // Called on receiving BEGIN_RESP or TLM_COMPLETED
    void check_transaction(tlm_generic_payload& trans) {
        if ( trans.is_response_error() ) {
            char txt[100];
            sprintf(txt, "Transaction returned with error, response status = %s",
            trans.get_response_string().c_str());
            SC_REPORT_ERROR("TLM-2", txt);
        }

        tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address();
        int*             ptr = reinterpret_cast<int*>( trans.get_data_ptr() );

        fout<< hex << adr << " check, cmd=" << (cmd ? 'W' : 'R') << ", data=" << hex << *ptr << " at time " << sc_time_stamp() << endl;

        // Allow the memory manager to free the transaction object
        trans.release();
    }
};
#endif
