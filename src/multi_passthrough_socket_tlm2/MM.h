#ifndef MM_H
#define MM_H
#include "systemc"

#include "tlm.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

//#define DEBUG

using namespace std;

class MM: public tlm::tlm_mm_interface {
    typedef tlm::tlm_generic_payload gp_t;

public:
    MM() :
        free_list(0),
        empties(0)
        #ifdef DEBUG
        , count(0)
        #endif
    {
        // empty constructor
    }

    gp_t* allocate();
    void  free(gp_t* trans);

    private:
        struct access {
            gp_t* trans;
            access* next;
            access* prev;
        };

        access* free_list;
        access* empties;

        #ifdef DEBUG
        int     count;
        #endif
    };

    MM::gp_t* MM::allocate() {
        #ifdef DEBUG
        fout << "----------------------------- Called allocate(), #trans = " << ++count << endl;
        #endif
        gp_t* ptr;
        if (free_list) {
            ptr = free_list->trans;
            empties = free_list;
            free_list = free_list->next;
        }
        else {
            ptr = new gp_t(this);
        }
        return ptr;
    }

    void MM::free(gp_t* trans) {
        #ifdef DEBUG
        fout << "----------------------------- Called free(), #trans = " << --count << endl;
        #endif
        if (!empties) {
            empties = new access();
            empties->next = free_list;
            empties->prev = 0;
            if (free_list) {
                free_list->prev = empties;
            }
        }
        free_list = empties;
        free_list->trans = trans;
        empties = free_list->prev;
    }
#endif
