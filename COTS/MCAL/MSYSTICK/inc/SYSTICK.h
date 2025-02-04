#ifndef SYSTICK_H
#define SYSTICK_H

savedSysClkHz = 0;
savedsystick_cbf = NULL;
systick_init(uint32_t systemClkHz)
{
    // step 1: save the system clock frequency
    savedSysClkHz = systemClkHz;

    // step 2: enable int
    // step 3: RELOAD -> 1ms
    // step 4: clear current counter val
}

systick_setcbf(systick_cbf)
{
    if (systick_cbf != NULL)
    {
        savedsystick_cbf = systick_cbf;
    }
}

systick_handler()
{
    passedMs++;
}

systick_delayms()
{
    
}
systick_setticktime();
systick_stop();
systick_start();

#endif