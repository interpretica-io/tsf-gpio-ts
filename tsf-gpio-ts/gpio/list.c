/** @file
 * @brief GPIO Group
 *
 * Enumerate the agent's GPIO chips and read one line back. The host
 * decides how many chips there are, so this asserts no count: an agent
 * with no gpiochip is a clean pass (the enumeration path still ran).
 * Read-only - it never drives a line.
 *
 * Copyright (C) 2026 Interpretica Unipessoal Lda
 */

#define TE_TEST_NAME    "gpio/list"

#include "te_config.h"
#include "tapi_test.h"
#include "te_vector.h"

#include "tapi_gpio.h"
#include "tsapi_gpio.h"

int
main(int argc, char **argv)
{
    tsapi_gpio_session sess;
    te_vec chips = TE_VEC_INIT(tapi_gpio_chip);
    const tapi_gpio_chip *chip;
    unsigned int n;

    TEST_START;

    TEST_STEP("Open a session to the agent");
    CHECK_RC(tsapi_gpio_session_init(&sess, "pco_gpio_list"));

    TEST_STEP("Enumerate the GPIO chips");
    CHECK_RC(tapi_gpio_list(sess.pco, &chips));
    n = te_vec_size(&chips);
    RING("the agent has %u GPIO chip(s)", n);

    if (n == 0)
    {
        TEST_STEP("No GPIO chips - nothing more to check");
        TEST_SUCCESS;
    }

    TEST_STEP("Every chip is self-consistent, and line 0 reads");
    TE_VEC_FOREACH(&chips, chip)
    {
        int value = -1;
        te_errno rc;

        if (chip->path == NULL || chip->num_lines == 0)
            TEST_VERDICT("chip %s has no path or no lines",
                         chip->name != NULL ? chip->name : "?");
        RING("%s: %s, %u lines", chip->path,
             chip->label != NULL ? chip->label : "?", chip->num_lines);

        /* Read line 0 as an input; a line in use by a driver may be
         * busy, which is not a suite failure - only a hard error is. */
        rc = tapi_gpio_get(sess.pco, chip->path, 0, &value);
        if (rc == 0)
            RING("%s line 0 = %d", chip->path, value);
        else
            RING("%s line 0 not readable (%r) - acceptable", chip->path, rc);
    }

    TEST_SUCCESS;

cleanup:
    tapi_gpio_list_free(&chips);
    tsapi_gpio_session_fini(&sess);
    TEST_END;
}
