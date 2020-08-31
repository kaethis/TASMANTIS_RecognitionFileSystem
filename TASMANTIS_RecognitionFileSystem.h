#ifndef _TASMANTIS_RECOGNITIONFILESYSTEM_H_

#define _TASMANTIS_RECOGNITIONFILESYSTEM_H_


#include <Arduino.h>

#include "TASMANTIS_RecognitionFileSystem.h"

#include "TASMANTIS_Recognition.h"

#include "TASMANTIS_FRAM.h"

#include "TASMANTIS_Math.h"

#include "TASMANTIS_Lidar.h"


class TASMANTIS_RecognitionFileSystem { /* -------------------------------------------------------------------------------------
* This CLASS ...
* --------------------------------------------------------------------------------------------------------------------------- */

  public:

    const static uint16_t ALLOC_ADDR = 0x00;

    const static uint16_t START_ADDR = 0x01;

    const static int PAGE_SIZE = 1332;

    const static int PAGE_N = 8;


    TASMANTIS_RecognitionFileSystem(TASMANTIS_FRAM*);


    void clear(void) { fram->writeByte(ALLOC_ADDR, 0x00); }

    void load(int, struct TASMANTIS_Math::pose*);

    void load(int, struct TASMANTIS_Math::pose*, struct TASMANTIS_Lidar::scan*, struct TASMANTIS_Recognition::freq*);

    void save(int, struct TASMANTIS_Math::pose*, struct TASMANTIS_Lidar::scan*, struct TASMANTIS_Recognition::freq*);

    int save(struct TASMANTIS_Math::pose*, struct TASMANTIS_Lidar::scan*, struct TASMANTIS_Recognition::freq*);

    int nextFree(void);

    void toggleFree(int);

    bool isFree(int);

    bool isFull(void);

    bool isEmpty(void);


  private:

    TASMANTIS_FRAM *fram;
};


#endif /* _TASMANTIS_RECOGNITIONFILESYSTEM_H_ */
