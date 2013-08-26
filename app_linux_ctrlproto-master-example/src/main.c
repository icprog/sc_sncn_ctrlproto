#include <ctrlproto_m.h>
#include <ecrt.h>
#include "ethercat_setup.h"
#include <stdio.h>
#include "profile.h"
#include "drive_function.h"
#include <motor_define.h>
#include <sys/time.h>
#include <time.h>

#define print_slave
void run_drive();



void set_controlword(int controlword)
{
	slv_handles[0].motorctrl_out = controlword;
}

int read_statusword()
{
	return slv_handles[0].motorctrl_status_in;
}


int main()
{
	int ready = 0;
	int switch_enable = 0;
	int status_word = 0;
	int switch_on_state = 0;
	int op_enable_state = 0;
	int control_word;
	int flag = 0;
	int v_d = -4000, u =0, acc= 1000, dec = 1000;
	int steps = 0, i = 1, target_velocity = 0;


//	printf("%d %d %d %d %d %d %d ", slv_handles[0].motor_config_param.s_gear_ratio.gear_ratio , slv_handles[0].motor_config_param.s_max_acceleration.max_acceleration\
//			, slv_handles[0].motor_config_param.s_nominal_current.nominal_current, slv_handles[0].motor_config_param.s_nominal_motor_speed.nominal_motor_speed,\
//			slv_handles[0].motor_config_param.s_polarity.polarity, slv_handles[0].motor_config_param.s_pole_pair.pole_pair, slv_handles[0].motor_config_param.s_position_encoder_resolution.position_encoder_resolution);

	init_master(&master_setup, slv_handles, NUM_SLAVES);

	printf("updating ");
	while(1)
	{
		if(slv_handles[0].motor_config_param.update_flag == 1)
		{
			pdo_handle_ecat(&master_setup, slv_handles, NUM_SLAVES);
			if(master_setup.opFlag)
			{
				slv_handles[0].motorctrl_out = 0xAA88;
				slv_handles[0].operation_mode = 0x35;
				slv_handles[0].torque_setpoint = 0x200;
				slv_handles[0].position_setpoint = 0x10000;
				slv_handles[0].speed_setpoint = 0x4000;
				printf("Status: %i ",slv_handles[0].motorctrl_status_in);
				printf("Position: %i ",slv_handles[0].position_in);
				printf("Speed: %i ",slv_handles[0].speed_in);
				printf("Torque: %i ",slv_handles[0].torque_in);
				printf("Operation Mode disp: %i\n",slv_handles[0].operation_mode_disp);
			}
		}
		else
		{

			sdo_handle_ecat(&master_setup, slv_handles, NUM_SLAVES);
			printf (".");
		}
	}

//	/**********************check ready***********************/
//	while(!ready)
//	{
//		handle_ecat(&master_setup,slv_handles, NUM_SLAVES);
//		if(master_setup.opFlag)
//		{
//			//check ready
//			status_word = read_statusword();
//			ready = check_ready(status_word);
//		}
//		else
//			continue;
//	}
//
//	#ifndef print_slave
//	printf("ready");
//	#endif
//
//	/**********************check switch_enable***********************/
//	while(!switch_enable)
//	{
//		handle_ecat(&master_setup,slv_handles, NUM_SLAVES);
//		if(master_setup.opFlag)
//		{
//			//check switch
//			status_word = read_statusword();
//			switch_enable = check_switch_enable(status_word);
//		}
//		else
//			continue;
//	}
//
//	#ifndef print_slave
//	printf("switch_enable");
//	#endif
//
//
//	/************************output switch on**************************/
//	while(!switch_on_state)
//	{
//		handle_ecat(&master_setup, slv_handles, NUM_SLAVES);
//		if(master_setup.opFlag)
//		{
//			set_controlword(SWITCH_ON_CONTROL);
//			/*************check switch_on_state***************/
//			status_word = read_statusword();
//			switch_on_state = check_switch_on(status_word);
//		}
//		else
//			continue;
//	}
//
//	#ifndef print_slave
//	printf("switch_on_state");
//	#endif
//
//	/*************************output enable op***********************/
//
//	while(!op_enable_state && master_setup.opFlag)
//	{
//		handle_ecat(&master_setup,slv_handles, NUM_SLAVES);
//		if(master_setup.opFlag)
//		{
//			set_controlword(ENABLE_OPERATION_CONTROL|QUICK_STOP_CONTROL);
//			/*************check op_enable_state**************/
//			status_word = read_statusword();
//			op_enable_state = check_op_enable(status_word);
//		}
//		else
//			continue;
//	}
//
//	#ifndef print_slave
//	printf("op_enable_state");
//	#endif
//
//	/**********************output Mode of Operation******************/
//	while(1)
//	{
//			handle_ecat(&master_setup,slv_handles, NUM_SLAVES);
//			if(master_setup.opFlag)
//			{
//				slv_handles[0].operation_mode = CSV;
//				/*************check operation_mode display**************/
//				//status_word = read_statusword();
//				//op_enable_state = check_op_enable(status_word);
//				if (slv_handles[0].operation_mode_disp == CSV)
//					break;
//			}
//			else
//				continue;
//	}
//
//	steps = init_velocity_profile(v_d, u, acc, dec);
//	while(1)
//	{
//		handle_ecat(&master_setup,slv_handles, NUM_SLAVES);
//
//		if(master_setup.opFlag)//Check if we are up
//		{
//			if(i<steps)
//				{
//					target_velocity = velocity_profile_generate(i);
//					slv_handles[0].speed_setpoint = target_velocity;
//					i = i+1;
//				}
//			if(i>=steps && flag == 0)
//			{
//				//printf("done");
//				u = slv_handles[0].speed_in; v_d =2000;
//				steps = init_velocity_profile(v_d, u, acc, dec);
//				i = 1;
//				flag = 1;
//			}
//			if(i>=steps && flag == 1)
//			{
//				u = slv_handles[0].speed_in; v_d =-1000;
//				steps = init_velocity_profile(v_d, u, acc, dec);
//				i = 1;
//				flag = 2;
//			}
//		}
//	}
	return 0;
}


/*
 	if(master_setup.opFlag)//Check if we are up
	{
		slv_handles[0].motorctrl_out = 12;
		slv_handles[0].torque_setpoint = 200;
		slv_handles[0].speed_setpoint = 4000;
		slv_handles[0].position_setpoint = 10000;
		slv_handles[0].operation_mode = 234;

		printf("Status: %i\n",slv_handles[0].motorctrl_status_in);
		printf("Position: %i\n",slv_handles[0].position_in);
		printf("Speed: %i\n",slv_handles[0].speed_in);
		printf("Torque: %i\n",slv_handles[0].torque_in);
		printf("Operation Mode disp: %i\n",slv_handles[0].operation_mode_disp);

	}
	else
	{
		//This is just a example what CAN be done when the master recognizes
		//that, for example, a slave is now missing...
		slv_handles[0].motorctrl_out= 0;
		slv_handles[0].torque_setpoint=0;
		slv_handles[0].speed_setpoint=0;
		slv_handles[0].position_setpoint=0;
		slv_handles[0].operation_mode=0;
	}
*/
