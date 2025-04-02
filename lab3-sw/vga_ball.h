#ifndef _VGA_BALL_H
#define _VGA_BALL_H

#include <linux/ioctl.h>

/* Struct for RGB color */
typedef struct {
    unsigned char red, green, blue;
} vga_ball_color_t;

/* Struct for H/V position */
typedef struct {
    unsigned short h;
    unsigned short v;
} vga_ball_hv_t;

/* Combined argument structure for ioctl */
typedef struct {
    vga_ball_color_t background;
    vga_ball_hv_t hv;
} vga_ball_arg_t;

#define VGA_BALL_MAGIC 'q'

/* IOCTLs */
#define VGA_BALL_WRITE_BACKGROUND _IOW(VGA_BALL_MAGIC, 1, vga_ball_arg_t)
#define VGA_BALL_READ_BACKGROUND  _IOR(VGA_BALL_MAGIC, 2, vga_ball_arg_t)
#define VGA_BALL_WRITE_HV         _IOW(VGA_BALL_MAGIC, 3, vga_ball_arg_t)
#define VGA_BALL_READ_HV          _IOR(VGA_BALL_MAGIC, 4, vga_ball_arg_t)

#endif /* _VGA_BALL_H */