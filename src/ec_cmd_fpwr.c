#include <stdio.h>
#include <stdlib.h>
#include <asm/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#include "ethercattype.h"
#include "fsm_slave.h"
#include "ecs_slave.h"
#include "ec_regs.h"

/** Configured Address Write */
void ec_cmd_fpwr(e_slave * slave)
{
	uint16_t wkc1;
	long val = 0;
	uint16_t ado = 0;
	uint16_t adp = 0;
	uint8_t *datagram = __ecat_frameheader(slave->pkt);
	uint16_t size = ec_dgram_size(slave->pkt);
	uint16_t datalen = ec_dgram_data_length(slave->pkt);
	uint8_t *data = ec_dgram_data(slave->pkt);
	uint16_t *wkc = (uint16_t *) & datagram[size];

	wkc1 = *wkc;
	wkc1++;
	*wkc = wkc1;
	ado = ec_dgram_ado(slave->pkt);
	adp = ec_dgram_adp(slave->pkt);

	printf("%s index=%d wkc=%d "
		"wkc1=%d ado=0x%x adp=0x%x "
		"station addr=0x%x "
		"datalen=%d\n",
	       __FUNCTION__, 
		slave->pkt_index, 
		*wkc, wkc1, 
		ado,adp,
		ec_station_address(),
		datalen);

	if (datalen == 0) {
		printf("insane no length\n");
		goto FPRD_OUT;
	}

	if (adp == ec_station_address()) {
		if (ado == ECT_REG_EEPSTAT){
			//ec_fsm_sii_state_start_reading
			ec_sii_start_read(data, datalen);
		} else{
			ec_raw_get_ado(ado, data, datalen);
		}
	}
FPRD_OUT:
	ecs_tx_packet(slave);
	__set_fsm_state(slave, ecs_rx_packet);
}
