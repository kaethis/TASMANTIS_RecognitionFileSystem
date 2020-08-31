#include "TASMANTIS_RecognitionFileSystem.h"


TASMANTIS_RecognitionFileSystem::TASMANTIS_RecognitionFileSystem(TASMANTIS_FRAM *fram) { /* ------------------------------------
* This CONSTRUCTOR ...
* --------------------------------------------------------------------------------------------------------------------------- */

    this->fram = fram;
}


void TASMANTIS_RecognitionFileSystem::load(int page_i, struct TASMANTIS_Math::pose *p) { /* ------------------------------------
* This FUNCTION ...
* --------------------------------------------------------------------------------------------------------------------------- */

    uint16_t addr,

             offset;


    offset = (START_ADDR + (PAGE_SIZE * (page_i - 1)));

    addr = offset;


    p->x = fram->readFloat(addr);

    addr += 4;


    p->y = fram->readFloat(addr);

    addr += 4;


    p->angle = fram->readFloat(addr);

    addr += 4;
}


void TASMANTIS_RecognitionFileSystem::load(int page_i,                              /* -----------------------------------------
* ------------------------------------- */ struct TASMANTIS_Math::pose *p,          /* -----------------------------------------
* ------------------------------------- */ struct TASMANTIS_Lidar::scan *s,         /* -----------------------------------------
* ------------------------------------- */ struct TASMANTIS_Recognition::freq *f) { /* -----------------------------------------
* This FUNCTION ...
* --------------------------------------------------------------------------------------------------------------------------- */

    uint16_t addr,

             offset;


    offset = (START_ADDR + (PAGE_SIZE * (page_i - 1)));

    addr = offset;


    p->x = fram->readFloat(addr);

    addr += 4;


    p->y = fram->readFloat(addr);

    addr += 4;


    p->angle = fram->readFloat(addr);

    addr += 4;


    for (int i = 0; i < 360; i++) {

        s->dists[i] = fram->readInteger(addr);

        addr += 2;
    }


    for (int i = 0; i < TASMANTIS_Recognition::DIST_MAX; i++) {

        f->counts[i] = fram->readByte(addr);

        addr++;
    }
}


void TASMANTIS_RecognitionFileSystem::save(int page_i,                              /* -----------------------------------------
* ------------------------------------ */  struct TASMANTIS_Math::pose *p,          /* -----------------------------------------
* ------------------------------------ */  struct TASMANTIS_Lidar::scan *s,         /* -----------------------------------------
* ------------------------------------ */  struct TASMANTIS_Recognition::freq *f) { /* -----------------------------------------
* This FUNCTION ...
* --------------------------------------------------------------------------------------------------------------------------- */

    uint16_t addr,

             offset;


    offset = (START_ADDR + (PAGE_SIZE * (page_i - 1)));

    addr = offset;


    fram->writeFloat(addr, p->x);

    addr += 4;


    fram->writeFloat(addr, p->y);

    addr += 4;


    fram->writeFloat(addr, p->angle);

    addr += 4;


    for (int i = 0; i < 360; i++) {

        fram->writeInteger(addr, s->dists[i]);

        addr += 2;
    }


    for (int i = 0; i < TASMANTIS_Recognition::DIST_MAX; i++) {

        fram->writeByte(addr, f->counts[i]);

        addr++;
    }


    toggleFree(page_i);
}


int TASMANTIS_RecognitionFileSystem::save(struct TASMANTIS_Math::pose *p,          /* ------------------------------------------ 
* ------------------------------------ */ struct TASMANTIS_Lidar::scan *s,         /* ------------------------------------------
* ------------------------------------ */ struct TASMANTIS_Recognition::freq *f) { /* ------------------------------------------
* This FUNCTION ...
* --------------------------------------------------------------------------------------------------------------------------- */

    int page_i;


    page_i = nextFree();

    save(page_i, p, s, f);


    return page_i;
}


bool TASMANTIS_RecognitionFileSystem::isFree(int page_i) { /* ------------------------------------------------------------------
* This FUNCTION ...
* --------------------------------------------------------------------------------------------------------------------------- */

    byte alloc,

         check;

    bool is_free;


    alloc = fram->readByte(ALLOC_ADDR);


    check = 0x01;

    check = (check << (page_i - 1));

    is_free = (!((alloc & check) == check));


    return is_free;
}


void TASMANTIS_RecognitionFileSystem::toggleFree(int page_i) { /* -----------------------------------------------------------------
* This FUNCTION ...
* --------------------------------------------------------------------------------------------------------------------------- */

    byte alloc;


    alloc = fram->readByte(ALLOC_ADDR);

    alloc = (alloc ^ (0x01 << (page_i - 1)));


    fram->writeByte(ALLOC_ADDR, alloc);
}


int TASMANTIS_RecognitionFileSystem::nextFree() { /* ---------------------------------------------------------------------------
* This FUNCTION ...
* --------------------------------------------------------------------------------------------------------------------------- */

    int page_i;


    for (page_i = 1; page_i <= PAGE_N; page_i++)

        if (isFree(page_i)) return page_i;


    return -1;
}


bool TASMANTIS_RecognitionFileSystem::isFull() { /* ----------------------------------------------------------------------------
* This FUNCTION ...
* --------------------------------------------------------------------------------------------------------------------------- */

    byte alloc;


    alloc = fram->readByte(ALLOC_ADDR);

    return ((alloc & 0xFF) == 0xFF);
}


bool TASMANTIS_RecognitionFileSystem::isEmpty() { /* ---------------------------------------------------------------------------
* This FUNCTION ...
* --------------------------------------------------------------------------------------------------------------------------- */

    byte alloc;


    alloc = fram->readByte(ALLOC_ADDR);

    return ((alloc & 0xFF) == 0x00);
}
