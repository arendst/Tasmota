/**
 * Copyright (C) 2021 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>

#include "bme68x.h"
#include "common.h"
#include "coines.h"

/***********************************************************************/
/*                         Test code                                   */
/***********************************************************************/

int main(void)
{
    struct bme68x_dev bme;
    int8_t rslt;

    /* Interface preference is updated as a parameter
     * For I2C : BME68X_I2C_INTF
     * For SPI : BME68X_SPI_INTF
     */
    rslt = bme68x_interface_init(&bme, BME68X_I2C_INTF);
    bme68x_check_rslt("bme68x_interface_init", rslt);

    rslt = bme68x_init(&bme);
    bme68x_check_rslt("bme68x_init", rslt);

    rslt = bme68x_selftest_check(&bme);
    bme68x_check_rslt("bme68x_selftest_check", rslt);

    if (rslt == BME68X_OK)
    {
        printf("Self-test passed\n");
    }

    if (rslt == BME68X_E_SELF_TEST)
    {
        printf("Self-test failed\n");
    }

    bme68x_coines_deinit();

    return rslt;
}
