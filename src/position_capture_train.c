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
	const unsigned int num_input_neurons = 2;
	const unsigned int num_output_neurons = 2;
	const unsigned int num_layers = 3;
	const unsigned int num_neurons_hidden_layer = 5;
	const float desired_error = (const float) 0;
	const unsigned int max_epochs = 50000;
	const unsigned int epochs_between_reports = 1000;
	struct fann *ann;
	struct fann_train_data *data;

	unsigned int i = 0;

	printf("Creating network.\n");
	ann = fann_create_standard(num_layers, num_input_neurons, num_neurons_hidden_layer, num_output_neurons);

	data = fann_read_train_from_file("../data/position_capture.data");

	fann_set_activation_steepness_hidden(ann, 1);
	fann_set_activation_steepness_output(ann, 1);

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	fann_set_train_stop_function(ann, FANN_STOPFUNC_BIT);
	fann_set_bit_fail_limit(ann, 0.01f);

	fann_set_training_algorithm(ann, FANN_TRAIN_INCREMENTAL);

	fann_init_weights(ann, data);
	
	printf("Training network.\n");
	fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error);

  /*
	printf("Testing network. %f\n", fann_test_data(ann, data));

	for(i = 0; i < fann_length_train_data(data); i++)
	{
		calc_out = fann_run(ann, data->input[i]);
		printf("Object position: Input: (%f,%f) -> ANN Output: (%f,%f), should be (%f,%f), difference=(%f,%f)\n",
        WIDTH*SCALE*data->input[i][0], HEIGHT*SCALE*data->input[i][1],
        WIDTH*SCALE*calc_out[0], HEIGHT*SCALE*calc_out[1],
        WIDTH*SCALE*data->output[i][0], HEIGHT*SCALE*data->output[i][1],
        WIDTH*SCALE*fann_abs(calc_out[0] - data->output[i][0]), HEIGHT*SCALE*fann_abs(calc_out[1] - data->output[i][1]));
	}
  */

	printf("Saving network.\n");

	fann_save(ann, "../ann/position_capture.net");

	printf("Cleaning up.\n");
	fann_destroy_train(data);
	fann_destroy(ann);

	return 0;
}
