#include"function.h"

#include <stdio.h>
#include <iostream>

//--- about camera -------------------------------------------
#include <fcntl.h>	// for ::open
#include <sys/ioctl.h> // for ioctl
#include <unistd.h>	// close
#include <string.h>	// memset
#include <linux/videodev2.h>	//for VIDIOC_S_INPUT
#include <sys/mman.h>

#include <apriltag/image_u8.h>

int OpenCamera(const char *pch_device);
int GetFrame(image_u8_t* pt_y_channel, image_u8_t* pt_u_channel, image_u8_t* pt_v_channel);

#define NUMBUFFERS 1
#define BYTEPERPIXEL 2		// temporary taking 1280*720 image from camera
#define CAMERAHEIGHT 720
#define CAMERAWIDTH 1280
#define PICHEIGHT 360
#define PICWIDTH 640

//--- about image -------------------------------------------
#include <memory>
struct SFrame {
	/* pointers to pixel data */
	std::unique_ptr<image_u8_t, void (*)(image_u8_t*)> Y, U, V;
	/* default constructor */
	SFrame() :
		Y(image_u8_create(0, 0), image_u8_destroy),
		U(image_u8_create(0, 0), image_u8_destroy),
		V(image_u8_create(0, 0), image_u8_destroy) {}
	/* set size of frame */
	void SetSize(size_t un_width, size_t un_height) {
		Y.reset(image_u8_create(un_width, un_height));
		U.reset(image_u8_create(un_width, un_height));
		V.reset(image_u8_create(un_width, un_height));
	}
};
SFrame currentFrame;

//------------------------------------------------------------

#include <chrono>  // for time measurement

///////////////////	functions  ///////////////////////////
int function_exit()
{
	exit(0);
	return 0;
}

int function_init()
{
	if (OpenCamera("/dev/video0") < 0) {printf("Camera Open Failed\n"); return -1;}
	currentFrame.SetSize(CAMERAWIDTH, CAMERAHEIGHT);

	return 0;
}

//------- for time measurement ---------------
auto t1 = std::chrono::high_resolution_clock::now();
auto t2= std::chrono::high_resolution_clock::now();
std::chrono::duration<double> elapsed;

int function_step()
{
	t2 = std::chrono::high_resolution_clock::now();

	if ( 
		GetFrame(currentFrame.Y.get(),currentFrame.U.get(),currentFrame.V.get())
		<0 ) printf("get Frame failed\n");

	elapsed = t2 - t1;
	t1 = t2;
	printf("duration = %lf\n",elapsed.count());


	//image_u8_write_pnm(currentFrame.Y.get(), "whatIhaveseen");

	return 0;
}

///////////////////	cameras	///////////////////////////
//--- about camera -------------------------------------------
int nCamera;	// the handle for the camera

int nCameraBytesPerPixel;
int nCameraWidth;
int nCameraHeight;
int nCameraFrameSize;

const int nNumBuffers = NUMBUFFERS;

struct OvFrameBuffer {
	unsigned char* start;
	unsigned int length;
	unsigned int offset;
};
OvFrameBuffer buffers_[NUMBUFFERS];
unsigned char* buffer_;

int OpenCamera(const char *pch_device) {
	/* open camera */
	int input;
	struct v4l2_format fmt;
	struct v4l2_streamparm parm;
	struct v4l2_requestbuffers req;

	/* TODO: change the following to be calculated from m_unImageWidth m_unImageHeight in the future*/
	nCameraWidth = CAMERAWIDTH;  
	nCameraHeight = CAMERAHEIGHT;  
	nCameraBytesPerPixel = BYTEPERPIXEL;
	nCameraFrameSize = nCameraWidth * nCameraHeight * nCameraBytesPerPixel;

	if ((nCamera = ::open(pch_device, O_RDWR, 0)) < 0)		// declared in fcntl.h
		return -1;

	input = 0;	//default input No 0
	if (ioctl(nCamera, VIDIOC_S_INPUT, &input) < 0) 
		{ close(nCamera); return -1; }

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = nCameraWidth;  
	fmt.fmt.pix.height = nCameraHeight;
	fmt.fmt.pix.sizeimage = nCameraFrameSize;
	fmt.fmt.pix.bytesperline = nCameraWidth * nCameraBytesPerPixel;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
	fmt.fmt.pix.field = V4L2_FIELD_NONE;
	buffer_ = new unsigned char[nCameraFrameSize];
	if (ioctl(nCamera, VIDIOC_S_FMT, &fmt) < 0)
		{ close(nCamera); return -1; }

	//--------------------------------------------------------------------
	/*
	int ret;
	ret = ioctl(nCamera, VIDIOC_G_PARM, &parm);
	if (ret < 0)
	{
		close(nCamera); printf("get par failed ret = %d\n",ret);
		printf("ioctl failed and returned errno %s \n",strerror(errno));
		//return -1;
	}
	parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	parm.parm.capture.timeperframe.numerator = 1;
	parm.parm.capture.timeperframe.denominator = 30;
	ret = ioctl(nCamera, VIDIOC_S_PARM, &parm);
	if (ret < 0)
	{
		close(nCamera); printf("set par failed ret = %d\n",ret); 
		printf("ioctl failed and returned errno %s \n",strerror(errno));
		return -1; 
	}
	*/
	//--------------------------------------------------------------------

	memset(&req, 0, sizeof (req));
	req.count = nNumBuffers;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory= V4L2_MEMORY_MMAP;
	if (ioctl (nCamera, VIDIOC_REQBUFS, &req) < 0) return -1;
	if (req.count < nNumBuffers) return -1;

	/* Start Capture */
	int i;
	struct v4l2_buffer buf;
	enum v4l2_buf_type type;

	for (i = 0; i < nNumBuffers; i++)
	{
		memset (&buf, 0, sizeof (buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if (ioctl(nCamera, VIDIOC_QUERYBUF, &buf) < 0) return -1;
		buffers_[i].length = buf.length;
		buffers_[i].offset = (size_t) buf.m.offset;
		buffers_[i].start = static_cast<unsigned char*>(
										mmap (NULL, buffers_[i].length, 
												PROT_READ | PROT_WRITE, MAP_SHARED, 
												nCamera, buffers_[i].offset
										)
								  );
		if (ioctl (nCamera, VIDIOC_QBUF, &buf) < 0) return -1;
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl (nCamera, VIDIOC_STREAMON, &type) < 0) return -1;

	return 0;
}

int GetFrame(image_u8_t* pt_y_channel, 
			image_u8_t* pt_u_channel, 
			image_u8_t* pt_v_channel) {
	struct v4l2_buffer capture_buf;
	//const unsigned int unScale = 2;
	struct SPixelData {
		unsigned char U0;
		unsigned char Y0;
		unsigned char V0;
		unsigned char Y1;
	};
	int current_buffer_index_;

	/* grab */
	memset (&capture_buf, 0, sizeof(capture_buf));

	capture_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	capture_buf.memory = V4L2_MEMORY_MMAP;
	if (ioctl(nCamera, VIDIOC_DQBUF, &capture_buf) < 0) return -1;

	buffer_ = buffers_[capture_buf.index].start;

	/* convert */
	/*
	unsigned int unDestIdx = 0, unPixelOffset = 0;
	unsigned int unStrideStep = pt_y_channel->stride - PICWIDTH;
	for (unsigned int unHeightIdx = 0; unHeightIdx < PICHEIGHT; unHeightIdx++) {
		for (unsigned int unWidthIdx = 0; unWidthIdx < PICWIDTH; unWidthIdx++) {
			SPixelData* psPixelData = reinterpret_cast<SPixelData*>(buffer_ + unPixelOffset);
			pt_u_channel->buf[unDestIdx] = psPixelData->U0;
			pt_y_channel->buf[unDestIdx] = psPixelData->Y0;
			pt_v_channel->buf[unDestIdx] = psPixelData->V0;

			unDestIdx++;
			unPixelOffset += nCameraBytesPerPixel;
			unPixelOffset += nCameraBytesPerPixel;
		}
		unDestIdx += unStrideStep;
		unPixelOffset += nCameraWidth * nCameraBytesPerPixel;
	}
	*/

	if (ioctl(nCamera, VIDIOC_QBUF, &capture_buf) < 0) return -1;

	return 0;
}
//end of camera------------------------------------------------------------

