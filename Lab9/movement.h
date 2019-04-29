/*
 * movement.h
 *
 *  Created on: Feb 4, 2019
 *      Author: vincentj
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

void move_init(int *event1);
void move_forward(oi_t *sensor, int millimeters);
int move_backward(oi_t *sensor_data, int millimeters);
void turn_right(oi_t *sensor_data, double degrees);
void turn_left(oi_t *sensor_data, double degrees);
void moving_in_square(oi_t *sensor_data);
void collision_detection(oi_t *sensor_data);
void plow_forward(oi_t *sensor_data, int millimeters);


#endif /* MOVEMENT_H_ */
