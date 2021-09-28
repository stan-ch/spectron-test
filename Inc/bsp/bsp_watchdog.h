/*  BSP Watchdog.
 *
 *  After init() watchdog will track tasks' counters. If they changed in proper
 *  time the harwdare watchog is refreshed, if at least one counter isn't
 *  changed - hardware wotchog is not refreshed and system will restart for a
 *  while.
 */

#ifndef BSP_WATCHDOG_H
#define BSP_WATCHDOG_H

#include "bsp_hal.h"

namespace bsp {
namespace watchdog {

//---------------------------------------------------------- Used resources ---

static IWDG_TypeDef* iwdg     [[gnu::unused]] = IWDG;

//-----------------------------------------------------------------------------

/** Initialize Watchdog with period specified by \a period_ms (milliseconds).
 */

void init();

/** Reset watchog timer.
 */

void refresh();

} // namespace watchdog
} // namespace bsp

#endif /* BSP_WATCHDOG_H */
