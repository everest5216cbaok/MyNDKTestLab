#ifndef GAME_H
#define GAME_H

#ifdef __cplusplus
extern "C" {
#endif

void on_surface_created();
void on_surface_changed();
void on_draw_frame();
void setUpdate(float * origin, float * target, int origin_length, int target_length, char * origin_pic_path, char * target_pic_path);

#ifdef __cplusplus
}
#endif

#endif
