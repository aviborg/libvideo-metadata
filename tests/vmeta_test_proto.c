/**
 * Copyright (c) 2016 Parrot Drones SAS
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the Parrot Drones SAS Company nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE PARROT DRONES SAS COMPANY BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "vmeta_test.h"

#ifndef MIN
#	define MIN(a, b)                                                      \
		({                                                             \
			__typeof__(a) _a = (a);                                \
			__typeof__(b) _b = (b);                                \
			_a < _b ? _a : _b;                                     \
		})
#endif


#define VMETA_ASSERT_NOT_NULL_FATAL(ptr, label)                                \
	({                                                                     \
		CU_ASSERT_PTR_NOT_NULL(ptr);                                   \
		if (!ptr)                                                      \
			goto label;                                            \
	})


#define VMETA_ASSERT_EQUAL_FATAL(a, b, label)                                  \
	({                                                                     \
		CU_ASSERT_EQUAL(a, b);                                         \
		if (a != b)                                                    \
			goto label;                                            \
	})


#define VMETA_ASSERT_BOTH_NULL_NOTNULL(p1, p2)                                 \
	CU_ASSERT((p1 == NULL && p2 == NULL) || (p1 && p2))


/**
 * This array can be generated by using the test executable with the 'dump'
 * argument. E.g.:
 * $ ./out/pdraw-linux/staging/native-wrapper.sh tst-vmeta dump
 * The generated data can be verified by dumping the binary data to a file and
 * using:
 * $ protoc --proto_path packages/libvideo-metadata/proto/ \
 *   packages/libvideo-metadata/proto/vmeta.proto --decode=vmeta.TimedMetadata \
 *   < dump.bin
 */

static const uint8_t packed_meta[] = {
	0x0a, 0x75, 0x0a, 0x14, 0x0d, 0xcd, 0xcc, 0xcc, 0x3d, 0x15, 0xcd, 0xcc,
	0x4c, 0x3e, 0x1d, 0x9a, 0x99, 0x99, 0x3e, 0x25, 0xcd, 0xcc, 0xcc, 0x3e,
	0x12, 0x1d, 0x09, 0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xb9, 0x3f, 0x11,
	0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xc9, 0x3f, 0x19, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x40, 0x7a, 0x40, 0x20, 0x0f, 0x19, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x50, 0x40, 0x22, 0x0f, 0x0d, 0x00, 0x00, 0x28, 0x42, 0x15,
	0x14, 0xae, 0x29, 0x42, 0x1d, 0x3d, 0x0a, 0xd7, 0x3e, 0x28, 0x54, 0x38,
	0x02, 0x4a, 0x0f, 0x0d, 0x00, 0x00, 0x28, 0x42, 0x15, 0x14, 0xae, 0x29,
	0x42, 0x1d, 0x3d, 0x0a, 0xd7, 0x3e, 0x52, 0x0f, 0x0d, 0x00, 0x00, 0x28,
	0x42, 0x15, 0x14, 0xae, 0x29, 0x42, 0x1d, 0x3d, 0x0a, 0xd7, 0x3e, 0x12,
	0x57, 0x08, 0xc2, 0x84, 0x89, 0x92, 0x04, 0x12, 0x14, 0x0d, 0x3d, 0x0a,
	0xd7, 0x3e, 0x15, 0xf6, 0x28, 0xdc, 0x3e, 0x1d, 0xae, 0x47, 0xe1, 0x3e,
	0x25, 0x66, 0x66, 0xe6, 0x3e, 0x1a, 0x14, 0x0d, 0x8f, 0xc2, 0xb5, 0x3f,
	0x15, 0x3d, 0x0a, 0xb7, 0x3f, 0x1d, 0xec, 0x51, 0xb8, 0x3f, 0x25, 0x9a,
	0x99, 0xb9, 0x3f, 0x25, 0x00, 0x00, 0x28, 0x42, 0x28, 0xb8, 0x17, 0x35,
	0x00, 0x00, 0x80, 0x40, 0x3d, 0x00, 0x00, 0x40, 0x40, 0x45, 0x00, 0x00,
	0x70, 0x42, 0x4d, 0x00, 0x00, 0x96, 0x42, 0x50, 0xa4, 0xc8, 0x90, 0xa1,
	0x02, 0x58, 0x88, 0x27, 0x1a, 0x08, 0x0a, 0x06, 0x08, 0x90, 0x4e, 0x18,
	0x9f, 0x01, 0x1a, 0x0b, 0x0a, 0x09, 0x08, 0xa0, 0x9c, 0x01, 0x10, 0x01,
	0x18, 0x8b, 0x01, 0x1a, 0x0e, 0x12, 0x0c, 0x0a, 0x06, 0x08, 0x03, 0x10,
	0x02, 0x20, 0x01, 0x0a, 0x02, 0x08, 0x04, 0x1a, 0x14, 0x12, 0x12, 0x0a,
	0x08, 0x08, 0x03, 0x10, 0x02, 0x18, 0x01, 0x20, 0x01, 0x0a, 0x04, 0x08,
	0x04, 0x18, 0x01, 0x10, 0x01, 0x22, 0x28, 0x0a, 0x1d, 0x0d, 0x29, 0x5c,
	0x0f, 0x3e, 0x15, 0x3d, 0x0a, 0x57, 0x3f, 0x1d, 0x9a, 0x99, 0x99, 0x3e,
	0x25, 0x0a, 0xd7, 0x23, 0x3c, 0x28, 0x05, 0x35, 0x00, 0x00, 0x00, 0x3f,
	0x38, 0x2a, 0x10, 0xc1, 0x82, 0x85, 0x8a, 0x04, 0x18, 0x80, 0x01, 0x2a,
	0x48, 0x0a, 0x02, 0x28, 0x03, 0x0a, 0x1d, 0x0d, 0x0a, 0xd7, 0x23, 0x3c,
	0x15, 0x0a, 0xd7, 0xa3, 0x3c, 0x1d, 0x8f, 0xc2, 0xf5, 0x3c, 0x25, 0x0a,
	0xd7, 0x23, 0x3d, 0x28, 0x03, 0x35, 0xcd, 0xcc, 0x4c, 0x3d, 0x38, 0x01,
	0x0a, 0x1d, 0x0d, 0x0a, 0xd7, 0xa3, 0x3c, 0x15, 0x0a, 0xd7, 0x23, 0x3d,
	0x1d, 0x8f, 0xc2, 0x75, 0x3d, 0x25, 0x0a, 0xd7, 0xa3, 0x3d, 0x28, 0x03,
	0x35, 0xcd, 0xcc, 0xcc, 0x3d, 0x38, 0x02, 0x10, 0xc0, 0x80, 0x81, 0x82,
	0x04,
};


static inline uint32_t randu32(void)
{
	uint32_t tmp;
	futils_random32(&tmp);
	return tmp;
}


static inline double randflt(void)
{
	union {
		uint32_t i;
		float f;
	} id;
	do {
		futils_random32(&id.i);
	} while (!isfinite(id.f));
	return id.f;
}


static inline uint64_t randu64(void)
{
	uint64_t tmp;
	futils_random64(&tmp);
	return tmp;
}


static inline double randdbl(void)
{
	union {
		uint64_t i;
		double d;
	} id;
	do {
		futils_random64(&id.i);
	} while (!isfinite(id.d));
	return id.d;
}


static int maybe(int random)
{
	return random ? randu32() % 2 : 1;
}


static struct vmeta_frame *unpacked_meta(int random)
{
	int err;
	struct vmeta_frame *frame;
	err = vmeta_frame_new(VMETA_FRAME_TYPE_PROTO, &frame);
	CU_ASSERT_PTR_NOT_NULL(frame);
	CU_ASSERT_EQUAL(err, 0);
	Vmeta__TimedMetadata *meta;
	err = vmeta_frame_proto_get_unpacked_rw(frame, &meta);
	CU_ASSERT_PTR_NOT_NULL(meta);
	CU_ASSERT_EQUAL(err, 0);

	if (maybe(random)) {
		Vmeta__DroneMetadata *drone = vmeta_frame_proto_get_drone(meta);
		CU_ASSERT_PTR_NOT_NULL(drone);
		if (random) {
			drone->battery_percentage = (int32_t)randu32();
			drone->flying_state = randu32();
			drone->ground_distance = randdbl();
		} else {
			drone->battery_percentage = 42;
			drone->flying_state = VMETA__FLYING_STATE__FS_HOVERING;
			drone->ground_distance = 0x42;
		}

		if (maybe(random)) {
			Vmeta__Quaternion *drone_quat =
				vmeta_frame_proto_get_drone_quat(drone);
			CU_ASSERT_PTR_NOT_NULL(drone_quat);
			if (random) {
				drone_quat->w = randflt();
				drone_quat->x = randflt();
				drone_quat->y = randflt();
				drone_quat->z = randflt();
			} else {
				drone_quat->w = 0.1f;
				drone_quat->x = 0.2f;
				drone_quat->y = 0.3f;
				drone_quat->z = 0.4f;
			}
		}

		if (maybe(random)) {
			Vmeta__Location *drone_loc =
				vmeta_frame_proto_get_drone_location(drone);
			CU_ASSERT_PTR_NOT_NULL(drone_loc);
			if (random) {
				drone_loc->altitude = randdbl();
				drone_loc->latitude = randdbl();
				drone_loc->longitude = randdbl();
				drone_loc->sv_count = randu32();
			} else {
				drone_loc->altitude = 420.;
				drone_loc->latitude = 0.1;
				drone_loc->longitude = 0.2;
				drone_loc->sv_count = 15;
			}
		}

		if (maybe(random)) {
			Vmeta__NED *drone_position =
				vmeta_frame_proto_get_drone_position(drone);
			CU_ASSERT_PTR_NOT_NULL(drone_position);
			if (random) {
				drone_position->north = randflt();
				drone_position->east = randflt();
				drone_position->down = randflt();
			} else {
				drone_position->north = 42.f;
				drone_position->east = 42.42f;
				drone_position->down = 0.42f;
			}
		}

		if (maybe(random)) {
			Vmeta__Vector3 *drone_local_position =
				vmeta_frame_proto_get_drone_local_position(
					drone);
			CU_ASSERT_PTR_NOT_NULL(drone_local_position);
			if (random) {
				drone_local_position->x = randflt();
				drone_local_position->y = randflt();
				drone_local_position->z = randflt();
			} else {
				drone_local_position->x = 42.f;
				drone_local_position->y = 42.42f;
				drone_local_position->z = 0.42f;
			}
		}

		if (maybe(random)) {
			Vmeta__NED *drone_speed =
				vmeta_frame_proto_get_drone_speed(drone);
			CU_ASSERT_PTR_NOT_NULL(drone_speed);
			if (random) {
				drone_speed->north = randflt();
				drone_speed->east = randflt();
				drone_speed->down = randflt();
			} else {
				drone_speed->north = 42.f;
				drone_speed->east = 42.42f;
				drone_speed->down = 0.42f;
			}
		}
	}

	if (maybe(random)) {
		Vmeta__CameraMetadata *camera =
			vmeta_frame_proto_get_camera(meta);
		CU_ASSERT_PTR_NOT_NULL(camera);
		if (random) {
			camera->timestamp = randu64();
			camera->utc_timestamp = randu64();
			camera->utc_timestamp_accuracy = randu32();
			camera->awb_b_gain = randflt();
			camera->awb_r_gain = randflt();
			camera->exposure_time = randflt();
			camera->hfov = randflt();
			camera->vfov = randflt();
			camera->iso_gain = randu32();
		} else {
			camera->timestamp = UINT64_C(0x42424242);
			camera->utc_timestamp = UINT64_C(0x24242424);
			camera->utc_timestamp_accuracy = 5000;
			camera->awb_b_gain = 3.f;
			camera->awb_r_gain = 4.f;
			camera->exposure_time = 42.f;
			camera->hfov = 60.f;
			camera->vfov = 75.f;
			camera->iso_gain = 3000;
		}

		if (maybe(random)) {
			Vmeta__Quaternion *camera_base_quat =
				vmeta_frame_proto_get_camera_base_quat(camera);
			CU_ASSERT_PTR_NOT_NULL(camera_base_quat);
			if (random) {
				camera_base_quat->w = randflt();
				camera_base_quat->x = randflt();
				camera_base_quat->y = randflt();
				camera_base_quat->z = randflt();
			} else {
				camera_base_quat->w = 0.42f;
				camera_base_quat->x = 0.43f;
				camera_base_quat->y = 0.44f;
				camera_base_quat->z = 0.45f;
			}
		}

		if (maybe(random)) {
			Vmeta__Quaternion *camera_quat =
				vmeta_frame_proto_get_camera_quat(camera);
			CU_ASSERT_PTR_NOT_NULL(camera_quat);
			if (random) {
				camera_quat->w = randflt();
				camera_quat->x = randflt();
				camera_quat->y = randflt();
				camera_quat->z = randflt();
			} else {
				camera_quat->w = 1.42f;
				camera_quat->x = 1.43f;
				camera_quat->y = 1.44f;
				camera_quat->z = 1.45f;
			}
		}
	}

	size_t n_links = random ? randu32() % 10 : 2;
	for (size_t i = 0; i < n_links; i++) {
		Vmeta__WifiLinkMetadata *wifi =
			vmeta_frame_proto_add_wifi_link(meta);
		CU_ASSERT_PTR_NOT_NULL(wifi);
		if (random) {
			wifi->goodput = randu32();
			wifi->quality = randu32();
			wifi->rssi = (int32_t)randu32();
			;
		} else {
			wifi->goodput = 10000 * (i + 1);
			wifi->quality = i;
			wifi->rssi = -80 + (10 * i);
		}
	}

	n_links = random ? randu32() % 10 : 2;
	for (size_t i = 0; i < n_links; i++) {
		Vmeta__StarfishLinkMetadata *starfish =
			vmeta_frame_proto_add_starfish_link(meta);
		CU_ASSERT_PTR_NOT_NULL(starfish);
		for (size_t j = 0; j < 2; j++) {
			Vmeta__StarfishLinkInfo *starfish_info =
				vmeta_frame_proto_add_starfish_link_info(
					starfish);
			starfish_info->type =
				(j == 0) ? VMETA__LINK_TYPE__LINK_TYPE_WLAN
					 : VMETA__LINK_TYPE__LINK_TYPE_CELLULAR;
			starfish_info->status =
				(j == 0)
					? VMETA__LINK_STATUS__LINK_STATUS_RUNNING
					: VMETA__LINK_STATUS__LINK_STATUS_DOWN;
			starfish_info->active = (j == 0);
			if (random)
				starfish_info->quality = randu32();
			else
				starfish_info->quality = i;
			if (starfish_info->active)
				starfish->quality = starfish_info->quality;
		}
	}

	if (maybe(random)) {
		Vmeta__TrackingMetadata *tracking =
			vmeta_frame_proto_get_tracking(meta);
		CU_ASSERT_PTR_NOT_NULL(tracking);
		if (random) {
			tracking->timestamp = randu64();
			tracking->quality = randu32();
			tracking->state = randu32();
		} else {
			tracking->timestamp = UINT64_C(0x41414141);
			tracking->quality = 128;
			tracking->state = VMETA__TRACKING_STATE__TS_TRACKING;
		}

		if (maybe(random)) {
			Vmeta__BoundingBox *box =
				vmeta_frame_proto_get_tracking_target(tracking);
			CU_ASSERT_PTR_NOT_NULL(box);
			if (random) {
				box->object_class = randu32();
				box->x = randflt();
				box->y = randflt();
				box->width = randflt();
				box->height = randflt();
				box->confidence = randflt();
				box->uid = randu32();
			} else {
				box->object_class =
					VMETA__TRACKING_CLASS__TC_HORSE;
				box->x = 0.14f;
				box->y = 0.84f;
				box->width = 0.3f;
				box->height = 0.01f;
				box->confidence = 0.5f;
				box->uid = 42;
			}
		}
	}

	if (maybe(random)) {
		Vmeta__TrackingProposalMetadata *proposal =
			vmeta_frame_proto_get_proposal(meta);
		CU_ASSERT_PTR_NOT_NULL(proposal);
		if (random)
			proposal->timestamp = randu64();
		else
			proposal->timestamp = UINT64_C(0x40404040);
		size_t nprops = random ? randu32() % 20 : 3;
		for (size_t i = 0; i < nprops; i++) {
			Vmeta__BoundingBox *box =
				vmeta_frame_proto_proposal_add_box(proposal);
			CU_ASSERT_PTR_NOT_NULL(box);
			if (random) {
				box->object_class = randu32();
				box->x = randflt();
				box->y = randflt();
				box->width = randflt();
				box->height = randflt();
				box->confidence = randflt();
				box->uid = randu32();
			} else {
				box->object_class =
					VMETA__TRACKING_CLASS__TC_BOAT;
				box->x = 0.01f * i;
				box->y = 0.02f * i;
				box->width = 0.03f * i;
				box->height = 0.04f * i;
				box->confidence = 0.05f * i;
				box->uid = i;
			}
		}
	}

	err = vmeta_frame_proto_release_unpacked_rw(frame, meta);
	CU_ASSERT_EQUAL(err, 0);

	return frame;
}


static void compare_quaternion(Vmeta__Quaternion *q1, Vmeta__Quaternion *q2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(q1, q2);
	if (!q1 || !q2)
		return;

	CU_ASSERT_EQUAL(q1->w, q2->w);
	CU_ASSERT_EQUAL(q1->x, q2->x);
	CU_ASSERT_EQUAL(q1->y, q2->y);
	CU_ASSERT_EQUAL(q1->z, q2->z);
}


static void compare_location(Vmeta__Location *l1, Vmeta__Location *l2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(l1, l2);
	if (!l1 || !l2)
		return;

	CU_ASSERT_EQUAL(l1->altitude, l2->altitude);
	CU_ASSERT_EQUAL(l1->latitude, l2->latitude);
	CU_ASSERT_EQUAL(l1->longitude, l2->longitude);
	CU_ASSERT_EQUAL(l1->sv_count, l2->sv_count);
}


static void compare_vector3(Vmeta__Vector3 *v1, Vmeta__Vector3 *v2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(v1, v2);
	if (!v1 || !v2)
		return;

	CU_ASSERT_EQUAL(v1->x, v2->x);
	CU_ASSERT_EQUAL(v1->y, v2->y);
	CU_ASSERT_EQUAL(v1->z, v2->z);
}


static void compare_ned(Vmeta__NED *n1, Vmeta__NED *n2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(n1, n2);
	if (!n1 || !n2)
		return;

	CU_ASSERT_EQUAL(n1->north, n2->north);
	CU_ASSERT_EQUAL(n1->east, n2->east);
	CU_ASSERT_EQUAL(n1->down, n2->down);
}


static void compare_bounding_box(Vmeta__BoundingBox *b1, Vmeta__BoundingBox *b2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(b1, b2);
	if (!b1 || !b2)
		return;

	CU_ASSERT_EQUAL(b1->object_class, b2->object_class);
	CU_ASSERT_EQUAL(b1->confidence, b2->confidence);
	CU_ASSERT_EQUAL(b1->height, b2->height);
	CU_ASSERT_EQUAL(b1->width, b2->width);
	CU_ASSERT_EQUAL(b1->x, b2->x);
	CU_ASSERT_EQUAL(b1->y, b2->y);
	CU_ASSERT_EQUAL(b1->uid, b2->uid);
}


static void compare_drone(Vmeta__DroneMetadata *d1, Vmeta__DroneMetadata *d2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(d1, d2);
	if (!d1 || !d2)
		return;

	CU_ASSERT_EQUAL(d1->battery_percentage, d2->battery_percentage);
	CU_ASSERT_EQUAL(d1->flying_state, d2->flying_state);
	CU_ASSERT_EQUAL(d1->ground_distance, d2->ground_distance);
	compare_quaternion(d1->quat, d2->quat);
	compare_ned(d1->position, d2->position);
	compare_ned(d1->speed, d2->speed);
	compare_vector3(d1->local_position, d2->local_position);
	compare_location(d1->location, d2->location);
}


static void compare_camera(Vmeta__CameraMetadata *c1, Vmeta__CameraMetadata *c2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(c1, c2);
	if (!c1 || !c2)
		return;

	CU_ASSERT_EQUAL(c1->awb_b_gain, c2->awb_b_gain);
	CU_ASSERT_EQUAL(c1->awb_r_gain, c2->awb_r_gain);
	CU_ASSERT_EQUAL(c1->exposure_time, c2->exposure_time);
	CU_ASSERT_EQUAL(c1->hfov, c2->hfov);
	CU_ASSERT_EQUAL(c1->iso_gain, c2->iso_gain);
	CU_ASSERT_EQUAL(c1->timestamp, c2->timestamp);
	CU_ASSERT_EQUAL(c1->utc_timestamp, c2->utc_timestamp);
	CU_ASSERT_EQUAL(c1->utc_timestamp_accuracy, c2->utc_timestamp_accuracy);
	CU_ASSERT_EQUAL(c1->vfov, c2->vfov);
	compare_quaternion(c1->base_quat, c2->base_quat);
	compare_quaternion(c1->quat, c2->quat);
}


static void compare_proposal(Vmeta__TrackingProposalMetadata *p1,
			     Vmeta__TrackingProposalMetadata *p2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(p1, p2);
	if (!p1 || !p2)
		return;

	CU_ASSERT_EQUAL(p1->n_proposals, p2->n_proposals);
	if (p1->n_proposals != p2->n_proposals)
		return;
	for (size_t i = 0; i < p1->n_proposals; i++)
		compare_bounding_box(p1->proposals[i], p2->proposals[i]);
}


static void compare_tracking(Vmeta__TrackingMetadata *t1,
			     Vmeta__TrackingMetadata *t2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(t1, t2);
	if (!t1 || !t2)
		return;

	CU_ASSERT_EQUAL(t1->quality, t2->quality);
	CU_ASSERT_EQUAL(t1->state, t2->state);
	compare_bounding_box(t1->target, t2->target);
}


static void compare_wifi(Vmeta__WifiLinkMetadata *w1,
			 Vmeta__WifiLinkMetadata *w2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(w1, w2);
	if (!w1 || !w2)
		return;

	CU_ASSERT_EQUAL(w1->goodput, w2->goodput);
	CU_ASSERT_EQUAL(w1->quality, w2->quality);
	CU_ASSERT_EQUAL(w1->rssi, w2->rssi);
}


static void compare_starfish_link_info(Vmeta__StarfishLinkInfo *s1,
				       Vmeta__StarfishLinkInfo *s2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(s1, s2);
	if (!s1 || !s2)
		return;

	CU_ASSERT_EQUAL(s1->type, s2->type);
	CU_ASSERT_EQUAL(s1->status, s2->status);
	CU_ASSERT_EQUAL(s1->quality, s2->quality);
	CU_ASSERT_EQUAL(s1->active, s2->active);
}


static void compare_starfish(Vmeta__StarfishLinkMetadata *s1,
			     Vmeta__StarfishLinkMetadata *s2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(s1, s2);
	if (!s1 || !s2)
		return;

	CU_ASSERT_EQUAL(s1->n_links, s2->n_links);
	if (s1->n_links == s2->n_links) {
		for (size_t i = 0; i < s2->n_links; i++)
			compare_starfish_link_info(s1->links[i], s2->links[i]);
	}
	CU_ASSERT_EQUAL(s1->quality, s2->quality);
}


static void compare_link(Vmeta__LinkMetadata *l1, Vmeta__LinkMetadata *l2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(l1, l2);
	if (!l1 || !l2)
		return;

	CU_ASSERT_EQUAL(l1->protocol_case, l2->protocol_case);
	if (l1->protocol_case != l2->protocol_case)
		return;

	switch (l1->protocol_case) {
	case VMETA__LINK_METADATA__PROTOCOL_WIFI:
		CU_ASSERT_PTR_NOT_NULL(l1->wifi);
		CU_ASSERT_PTR_NOT_NULL(l2->wifi);
		compare_wifi(l1->wifi, l2->wifi);
		break;
	case VMETA__LINK_METADATA__PROTOCOL_STARFISH:
		CU_ASSERT_PTR_NOT_NULL(l1->starfish);
		CU_ASSERT_PTR_NOT_NULL(l2->starfish);
		compare_starfish(l1->starfish, l2->starfish);
		break;
	default:
		break;
	}
}


static void compare_timed_metadata(const Vmeta__TimedMetadata *t1,
				   const Vmeta__TimedMetadata *t2)
{
	VMETA_ASSERT_BOTH_NULL_NOTNULL(t1, t2);
	if (!t1 || !t2)
		return;

	compare_drone(t1->drone, t2->drone);
	compare_camera(t1->camera, t2->camera);
	CU_ASSERT_EQUAL(t1->n_links, t2->n_links);
	if (t1->n_links == t2->n_links) {
		for (size_t i = 0; i < t1->n_links; i++)
			compare_link(t1->links[i], t2->links[i]);
	}
	compare_tracking(t1->tracking, t2->tracking);
	compare_proposal(t1->proposal, t2->proposal);
}


static void meta_compare(struct vmeta_frame *f1, struct vmeta_frame *f2)
{
	int err;
	CU_ASSERT_PTR_NOT_NULL(f1);
	CU_ASSERT_PTR_NOT_NULL(f2);
	if (!f1 || !f2)
		return;

	CU_ASSERT_EQUAL(f1->type, VMETA_FRAME_TYPE_PROTO);
	CU_ASSERT_EQUAL(f2->type, VMETA_FRAME_TYPE_PROTO);

	const Vmeta__TimedMetadata *m1, *m2;
	err = vmeta_frame_proto_get_unpacked(f1, &m1);
	CU_ASSERT_EQUAL(err, 0);
	CU_ASSERT_PTR_NOT_NULL(m1);
	err = vmeta_frame_proto_get_unpacked(f2, &m2);
	CU_ASSERT_EQUAL(err, 0);
	CU_ASSERT_PTR_NOT_NULL(m2);
	compare_timed_metadata(m1, m2);
	err = vmeta_frame_proto_release_unpacked(f1, m1);
	CU_ASSERT_EQUAL(err, 0);
	err = vmeta_frame_proto_release_unpacked(f2, m2);
	CU_ASSERT_EQUAL(err, 0);
}


static void test_api(void)
{
	int res;
	struct vmeta_frame *frame = NULL;
	const Vmeta__TimedMetadata *ctm;
	Vmeta__TimedMetadata *tm, *tm2;
	const uint8_t *buf;
	size_t len;

	/* Create a metadata */
	frame = unpacked_meta(0);
	CU_ASSERT_PTR_NOT_NULL(frame);

	/* check others getters in simple mode */
	res = vmeta_frame_proto_get_unpacked(frame, &ctm);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_PTR_NOT_NULL(ctm);
	res = vmeta_frame_proto_release_unpacked(frame, ctm);
	CU_ASSERT_EQUAL(res, 0);

	res = vmeta_frame_proto_get_unpacked_rw(frame, &tm);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_PTR_NOT_NULL(tm);
	res = vmeta_frame_proto_release_unpacked_rw(frame, tm);
	CU_ASSERT_EQUAL(res, 0);

	res = vmeta_frame_proto_get_buffer(frame, &buf, &len);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_PTR_NOT_NULL(buf);
	res = vmeta_frame_proto_release_buffer(frame, buf);
	CU_ASSERT_EQUAL(res, 0);

	/* Multiple read-lock calls */
	res = vmeta_frame_proto_get_unpacked(frame, &ctm);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_PTR_NOT_NULL(ctm);
	res = vmeta_frame_proto_get_buffer(frame, &buf, &len);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_PTR_NOT_NULL(buf);
	res = vmeta_frame_proto_get_unpacked_rw(frame, &tm);
	CU_ASSERT_EQUAL(res, -EBUSY);
	res = vmeta_frame_proto_release_unpacked(frame, ctm);
	CU_ASSERT_EQUAL(res, 0);
	res = vmeta_frame_proto_release_buffer(frame, buf);
	CU_ASSERT_EQUAL(res, 0);

	/* write-lock */
	res = vmeta_frame_proto_get_unpacked_rw(frame, &tm);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_PTR_NOT_NULL(tm);
	res = vmeta_frame_proto_get_unpacked(frame, &ctm);
	CU_ASSERT_EQUAL(res, -EBUSY);
	res = vmeta_frame_proto_get_buffer(frame, &buf, &len);
	CU_ASSERT_EQUAL(res, -EBUSY);
	res = vmeta_frame_proto_get_unpacked_rw(frame, &tm2);
	CU_ASSERT_EQUAL(res, -EBUSY);
	res = vmeta_frame_proto_release_unpacked_rw(frame, tm);
	CU_ASSERT_EQUAL(res, 0);

	res = vmeta_frame_unref(frame);
	CU_ASSERT_EQUAL(res, 0);
}


static void test_write(void)
{
	struct vmeta_frame *frame = NULL;
	const size_t buflen = 1024;
	uint8_t *buf = NULL;
	struct vmeta_buffer vb;
	int res;

	frame = unpacked_meta(0);
	CU_ASSERT_PTR_NOT_NULL(frame);

	buf = malloc(buflen);
	CU_ASSERT_PTR_NOT_NULL(buf);
	vmeta_buffer_set_data(&vb, buf, buflen, 0);

	res = vmeta_frame_write(&vb, frame);
	CU_ASSERT_EQUAL(res, 0);
	CU_ASSERT_EQUAL(vb.pos, sizeof(packed_meta));
	CU_ASSERT_EQUAL(memcmp(buf, packed_meta, sizeof(packed_meta)), 0);

	free(buf);
	vmeta_frame_unref(frame);
}


static void test_read(void)
{
	struct vmeta_frame *frame, *ref;
	struct vmeta_buffer vb;
	int err;

	vmeta_buffer_set_cdata(&vb, packed_meta, sizeof(packed_meta), 0);

	ref = unpacked_meta(0);
	CU_ASSERT_PTR_NOT_NULL(ref);
	err = vmeta_frame_read(&vb, VMETA_FRAME_PROTO_MIME_TYPE, &frame);
	CU_ASSERT_PTR_NOT_NULL(frame);
	CU_ASSERT_EQUAL(err, 0);

	meta_compare(frame, ref);

	vmeta_frame_unref(frame);
	vmeta_frame_unref(ref);
}


static void test_write_read_once(void)
{
	struct vmeta_frame *in, *out;
	uint8_t *buf;
	const size_t buflen = 1 * 1024 * 1024; /* 1M should be enough */
	struct vmeta_buffer vb;
	int res, err;

	buf = malloc(buflen);
	CU_ASSERT_PTR_NOT_NULL(buf);
	vmeta_buffer_set_data(&vb, buf, buflen, 0);

	in = unpacked_meta(1);
	CU_ASSERT_PTR_NOT_NULL(in);

	res = vmeta_frame_write(&vb, in);
	CU_ASSERT_EQUAL(res, 0);

	vb.len = vb.pos;
	vb.pos = 0;
	err = vmeta_frame_read(&vb, VMETA_FRAME_PROTO_MIME_TYPE, &out);
	CU_ASSERT_PTR_NOT_NULL(out);
	CU_ASSERT_EQUAL(err, 0);

	meta_compare(in, out);

	free(buf);
	vmeta_frame_unref(in);
	vmeta_frame_unref(out);
}


static void test_write_read(void)
{
	/* Since write_read test is random, do it multiple times */
	for (int i = 0; i < 1000; i++)
		test_write_read_once();
}


static void test_read_write(void)
{
	struct vmeta_frame *tmp;
	uint8_t *buf;
	const size_t buflen = 1024;
	struct vmeta_buffer in, out;
	int err;

	buf = malloc(buflen);
	CU_ASSERT_PTR_NOT_NULL(buf);
	vmeta_buffer_set_data(&out, buf, buflen, 0);
	vmeta_buffer_set_cdata(&in, packed_meta, sizeof(packed_meta), 0);

	err = vmeta_frame_read(&in, VMETA_FRAME_PROTO_MIME_TYPE, &tmp);
	CU_ASSERT_PTR_NOT_NULL(tmp);
	CU_ASSERT_EQUAL(err, 0);

	err = vmeta_frame_write(&out, tmp);
	CU_ASSERT_EQUAL(err, 0);

	CU_ASSERT_EQUAL(in.len, out.pos);
	CU_ASSERT_EQUAL(memcmp(in.data, out.data, in.len), 0);

	free(buf);
	vmeta_frame_unref(tmp);
}


static void gen_packed_meta(void)
{
	int res = 0;
	const uint8_t *buf;
	size_t len;

	struct vmeta_frame *ref = unpacked_meta(0);
	CU_ASSERT_PTR_NOT_NULL_FATAL(ref);
	res = vmeta_frame_proto_get_buffer(ref, &buf, &len);
	CU_ASSERT_EQUAL_FATAL(res, 0);

	fprintf(stderr, "\n\nstatic const uint8_t packed_meta[] = {\n\t");
	for (size_t i = 0; i < len; i++) {
		fprintf(stderr, "0x%02x,", buf[i]);
		if ((i % 12) == 11 && i < (len - 1))
			fprintf(stderr, "\n\t");
		else if (i < (len - 1))
			fprintf(stderr, " ");
	}
	fprintf(stderr, "\n};\n\n");

	vmeta_frame_proto_release_buffer(ref, buf);
	vmeta_frame_unref(ref);
}


CU_TestInfo s_proto_tests[] = {
	{(char *)"vmeta api", &test_api},
	{(char *)"vmeta write", &test_write},
	{(char *)"vmeta read", &test_read},
	{(char *)"vmeta read->write", &test_read_write},
	CU_TEST_INFO_NULL,
};

CU_TestInfo s_proto_monkey[] = {
	{(char *)"vmeta write->read monkey tests", &test_write_read},
	CU_TEST_INFO_NULL,
};

CU_TestInfo s_proto_gen[] = {
	{(char *)"vmeta packed_meta generation", &gen_packed_meta},
	CU_TEST_INFO_NULL,
};
