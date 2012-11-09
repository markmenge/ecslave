#include <stdio.h>
#include <string.h>
#include "ethercattype.h"
#include "fsm_slave.h"
#include "ecs_slave.h"
#include "ec_regs.h"
#include "ec_process_data.h"

void ec_dump_string(uint8_t *c,int len)
{
	int i ;

	for  (i = 0 ; i < len ; i++){
		printf("%02X ", c[i]);
	}
	puts("");
}

/** Logical Read Write */
void ec_cmd_lrw(e_slave * ecs,uint8_t *dgram_ec)
{
	uint16_t datalen = __ec_dgram_dlength(dgram_ec);
	uint8_t *data = __ec_dgram_data(dgram_ec);
	uint32_t offset = __ec_dgram_laddr(dgram_ec);
	uint8_t temp[datalen];

	get_process_data(temp, offset, datalen);
	set_process_data(data, offset, datalen);
	memcpy(data, temp, datalen);
#ifdef __MAKE_DEBUG__
	ec_dump_string(data, datalen);
#endif
       // If LRW is used, output FMMUs increment the working counter by 2,
        // while input FMMUs increment it by 1.
	__ec_inc_wkc__(dgram_ec);
	__ec_inc_wkc__(dgram_ec);
	__ec_inc_wkc__(dgram_ec);

     __set_fsm_state(ecs, ecs_process_next_dgram);
}
