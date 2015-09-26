/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2012 Steffen Nissen (sn@leenissen.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include "fann.h"

#define SCALE 1.3
#define WIDTH 640
#define HEIGHT 480

int main()
{
	fann_type *calc_out;
	unsigned int i;
	int ret = 0;

	struct fann *ann;
	struct fann_train_data *data;

	printf("Creating network.\n");

	ann = fann_create_from_file("../ann/position_capture.net");

	if(!ann)
	{
		printf("Error creating ann --- ABORTING.\n");
		return -1;
	}

	fann_print_connections(ann);
	fann_print_parameters(ann);

	printf("Testing network.\n");

	data = fann_read_train_from_file("../data/position_capture.data");

	for(i = 0; i < fann_length_train_data(data); i++)
	{
		calc_out = fann_run(ann, data->input[i]);
		printf("Object position: Input: (%f,%f) -> ANN Output: (%f,%f), should be (%f,%f), difference=(%f,%f)\n",
        WIDTH*SCALE*data->input[i][0], HEIGHT*SCALE*data->input[i][1],
        WIDTH*SCALE*calc_out[0], HEIGHT*SCALE*calc_out[1],
        WIDTH*SCALE*data->output[i][0], HEIGHT*SCALE*data->output[i][1],
        WIDTH*SCALE*fann_abs(calc_out[0] - data->output[i][0]), HEIGHT*SCALE*fann_abs(calc_out[1] - data->output[i][1]));
	}

	printf("Cleaning up.\n");
	fann_destroy_train(data);
	fann_destroy(ann);

	return ret;
}
