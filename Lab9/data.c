#include "lcd.h"
#include "open_interface.h"
#include "lab5_scan_data.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"

/**
 * @Author Vincent Johnson && Joshua French
 * Program to analyze:
 *  -Distance to an object
 *  -Angular Location of the object
 *  -Radial size of the object (the number within a sweep in which the object apperears)
 *  -Linear width (the actual width of the object)
 *
 */

void scan() {

    double ir_boundary = 80.0; //cm
    double sonar_boundary = 185.0; //cm
    double calibration = 8.5;

	int start_angle = 0;
	int end_angle = 0;
	
	double prev_ir = 0;
	double prev_sonar[180] = 0;

    //Find objects using sonar, then compare with IR

    //Once an object is found, use IR to find the angle (angular location)

    //Then find the radial size of the object

    //Then the linear width of the object, formula
	
	servo_move(0);
	timer_waitMillis(500);
	
	
	//scan
	int i = 0;
//	for (i = 0; i < 180; i++) {
//		double cur_ir = adc_conversion(adc_read(), calibration);
//		double cur_sonar = ping_getDistance() * .0010625;
//
//		//Object Start
//		if(prev_ir == ir_boundary && cur_ir < ir_boundary) {
//			start_angle = i;
//		}
//
//		//Object End
//		if ((prev_ir < ir_boundary && cur_ir == ir_boundary)&&(cur_ir < ir_boundary && i >= 179)) {
//			{
//				end_angle = i;
//
//				double arc_width = abs(end_angle - start_angle);
//				double object_angle = start_angle + (arc_width / 2);
//				double object_distance = prev_sonar[object_angle] - 1;
//				double object_width = ;
//
//
//				//Ignore small objects less than 2.0 cm
//				if (object_distance < ir_boundary && object_width > 2.0) {
//
//				}
//			}
//		}
//
//		prev_sonar[i] = cur_sonar;
//		prev_ir = cur_ir;
		
		servo_move(i);
		timer_wailMillis(1);
//	}


}
