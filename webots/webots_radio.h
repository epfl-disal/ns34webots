/*
 * Copyright (c) 2017 EPFL TUD
 * Distributed Intelligent Systems and Algorithms Laboratory,
 * Ecole polytechnique fédérale de Lausanne
 * Technische Universität, Dresden
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors: Ignacio Llatser <llatser@gmail.com>,
 * David Mansolino <david.mansolino@cyberbotics.com>
 */

#ifndef WEBOTS_RADIO_H
#define WEBOTS_RADIO_H

#ifdef __cplusplus
extern "C" {
#endif


// event types for WebotsRadioEvent
#define WEBOTS_RADIO_EVENT_RECEIVE          1
#define WEBOTS_RADIO_EVENT_NETWORK_DETECTED 2
#define WEBOTS_RADIO_EVENT_LOST_CONNECTION  3

struct WebotsRadioEvent {
  int         type;       // see above the WEBOTS_RADIO_EVENT_* type list
  const void *data;       // data chunk received by the radio
  int         data_size;  // size of the data chunk
  const char *from;       // address of the emitter of the data
  float       rssi;       // wireless networking parameter
  void       *user_data;  // user pointer defined in radio_set_callback()
};


void webots_radio_init();              /* initialization routine, to be called
                                          prior to any function call */

void webots_radio_cleanup();           /* cleanup routine, no further
                                          webots_radio_xxx() function can be
                                          called after this one */

int webots_radio_new();                /* create a new radio node */


/* parameter setters */
void webots_radio_set_alpha(double alpha);
void webots_radio_set_protocol(int radio,const char *protocol);
void webots_radio_set_address(int radio,const char *address);
void webots_radio_set_frequency(int radio,double frequency);
void webots_radio_set_channel(int radio,int channel);
void webots_radio_set_bitrate(int radio,double bitrate);
void webots_radio_set_rx_sensitivity(int radio,double rx_sensitivity);
void webots_radio_set_tx_power(int radio,double tx_power);
void webots_radio_set_callback(int radio,void *user_data,
                               void(*)(const int radio, const struct WebotsRadioEvent *));


/* move a radio node */
void webots_radio_move(int radio, double x, double y, double z);
struct xz {float x; float z;};
struct xz webots_radio_pos(int radio);

void webots_radio_send(int radio,const char *dest, const void *data, int size, double delay);

void webots_radio_delete(int radio);    /* delete a radio node */

void webots_radio_run(double seconds);   /* run the network simulation for
                                           a given number of seconds */

/* radio connected mode: to be implemented later on */

int  webots_radio_connection_open(int radio,const char *destination);
int  webots_radio_connection_send(int connection,const char *data,int size);
void webots_radio_connection_set_callback(int radio,void *ptr,
                                          void(*receive_func)(void *ptr,
                                                              char *data,
                                                              int size));
void webots_radio_connection_close(int connection);



#ifdef __cplusplus
}
#endif

#endif /* WEBOTS_RADIO_H */

