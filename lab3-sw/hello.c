/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int vga_ball_fd;

/* Read and print the background color */
void print_background_color() {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_READ_BACKGROUND) failed");
      return;
  }
  printf("%02x %02x %02x\n",
	 vla.background.red, vla.background.green, vla.background.blue);
}

/* Set the background color */
void set_background_color(const vga_ball_color_t *c)
{
  vga_ball_arg_t vla;
  vla.background = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_BACKGROUND, &vla)) {
      perror("ioctl(VGA_BALL_SET_BACKGROUND) failed");
      return;
  }
}

//********
/* Sets the horizontal (h) and vertical (v) position of the ball on the display */
void set_hv(const vga_ball_hv_t *c)
{
  vga_ball_arg_t vla;
  vla.hv = *c;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_HV, &vla)) {
      perror("ioctl(VGA_BALL_SET_HV) failed");
      return;
  }
}

int main()
{
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";

  static const vga_ball_color_t colors[] = {
    { 0xff, 0x00, 0x00 }, /* Red */
    { 0x00, 0xff, 0x00 }, /* Green */
    { 0x00, 0x00, 0xff }, /* Blue */
    { 0xff, 0xff, 0x00 }, /* Yellow */
    { 0x00, 0xff, 0xff }, /* Cyan */
    { 0xff, 0x00, 0xff }, /* Magenta */
    { 0x80, 0x80, 0x80 }, /* Gray */
    { 0x00, 0x00, 0x00 }, /* Black */
    { 0xff, 0xff, 0xff }  /* White */
  };

  // Struct to store horizontal and vertical position
  vga_ball_hv_t hv_val = {0, 0};

# define COLORS 9

  printf("VGA ball Userspace program started\n");

  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  printf("initial state: ");
  print_background_color();  //can comment out if needed

  //*******

  //Initialize parameters for motion
  unsigned int radius = 10;
  //horizatonal position
  int h_count = radius;
  //veritical position
  int v_count = radius;

  // Direction flag for horizontal movement
  bool h_flag = false;
  // Direction flag for vertical movement
  bool v_flag = false;

  // Main loop simulates bouncing ball movement
  for (i = 0 ; i < 20000; i++) {
    set_background_color(&colors[2]);

    //update position
    hv_val.h = h_count;
    hv_val.v = v_count;

    // Vertical movement logic
    if(v_count < 480-radius &&v_flag == false){
	      v_count++; // move down
    }
 
    if(v_count >= 480-radius || v_flag == true){
	      v_count--; // move up
        v_flag = true; // change direction to up
       if(v_count == radius)
	        v_flag = false; // bounce down again
    }

     // Horizontal movement logic
    if(h_count < 640-radius && h_flag == false){
        h_count++; // move right
    }
    if(h_count >= 640-radius || h_flag == true){
        h_count--; // move left
	      h_flag = true;  // set direction to left
	    if(h_count == radius)
	      h_flag = false;    // bounce right again
    }

    // // Send updated position to the device
    set_hv(&hv_val);

    // Optional: print color state for debugging; this is blue 
    print_background_color();
    // Delay to control speed of animation
    usleep(9000);
    
  }
  printf("VGA BALL Userspace program terminating\n");
  return 0;
}