"""
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
"""

def anim_zoom_cb(obj, v):
    obj.set_zoom(v)

# stop image
screen_img_1_img = screen_img_1.get_src()
# play image
screen_img_2_img = screen_img_2.get_src()

def screen_img_album_update_src(current, next):
    demoMP.img_dsc[next].clear_flag(lv.obj.FLAG.HIDDEN)
    demoMP.img_dsc[current].add_flag(lv.obj.FLAG.HIDDEN)
    demoMP.img_dsc[next].invalidate()
    demoMP.img_dsc[current].invalidate()

class MusicPlayer():
    def __init__(self) -> None:
        self.active_track_count = 3
        self.current_track_idx = 0
        self.slider_time = 0
        self.show_track_list = False
        self.playing = False
        self.title = [
            "Waiting for true love",
            "Need a Better Future",
            "Vibrations",
            "Why now",
            "Never Look Back",
            "It happened Yesterday",
            "Feeling so High",
            "Go Deeper",
        ]
        
        self.author = [
            "The John Smith Band",
            "My True Name",
            "Robotics",
            "John Smith",
            "My true Name",
            "Robotics",
            "Robotics",
            "Unknown artist",
        ]
        
        self.time = [
            1*60 + 14,
            2*60 + 26,
            1*60 + 54,
            2*60 + 24,
            2*60 + 57,
            2*60 + 33,
            3*60 + 33,
            1*60 + 56,
        ]
        
        self.btn = [
            screen_btn_1,
            screen_btn_2,
            screen_btn_3,
            screen_btn_4,
            screen_btn_5,
            screen_btn_6,
            screen_btn_7,
            screen_btn_8,
        ]
        
        self.icon = [
            screen_img_1,
            screen_img_2,
            screen_img_3,
            screen_img_4,
            screen_img_5,
            screen_img_6,
            screen_img_7,
            screen_img_8,
        ]
        
        self.img_dsc = [
            screen_img_album,
            screen_img_9,
            screen_img_10,
        ]
        
    def pause_track(self):
        #spectrum_i_pause = spectrum_i
        #spectrum_i = 0
        #lv_anim_del(spectrum_area, spectrum_anim_cb)
        #lv_obj_invalidate(spectrum_area)
        screen_img_album.set_zoom(lv.IMG_ZOOM.NONE)
        sec_counter_timer.pause()
        screen_imgbtn_play.clear_state(lv.STATE.CHECKED)
        screen_imgbtn_play.invalidate()
        
    def resume_track(self):
        #spectrum_i = spectrum_i_pause
        #a = lv.anim_t()
        #a.init()
        #a.set_var(screen_player)
        #a.set_time(1000)
        #a.set_path_cb(lv.anim_t.path_linear)
        #a.set_custom_exec_cb(lambda a,val: anim_y_cb(screen_player,val))
        #lv.anim_t.start(a)
        #
        #lv_anim_set_values(&a, spectrum_i, spectrum_len - 1)
        #lv_anim_set_exec_cb(&a, spectrum_anim_cb)
        #lv_anim_set_var(&a, spectrum_area)
        #lv_anim_set_time(&a, ((spectrum_len - spectrum_i) * 1000) / 30)
        #lv_anim_set_playback_time(&a, 0)
        #lv_anim_set_ready_cb(&a, spectrum_end_cb)
        #lv_anim_start(&a)
        sec_counter_timer.resume()
        screen_imgbtn_play.add_state(lv.STATE.CHECKED)
        screen_imgbtn_play.invalidate()
        
    def load_next_track(self, next=None, forward=True):
        if self.current_track_idx == next:
            return
        self.slider_time = 0
        #spectrum_i = 0
        #spectrum_i_pause = 0
        screen_slider_1.set_value(0, lv.ANIM.OFF)
        screen_label_slider_time.set_text("0:00")
        
        current = self.current_track_idx
        btn = self.btn[current]
        icon = self.icon[current]
        btn.clear_state(lv.STATE.PRESSED)
        icon.set_src(screen_img_2_img)
        #screen_img_album.fade_out(500, 0)
        #a = lv.anim_t()
        #a.init()
        #a.set_var(screen_img_album)
        #a.set_time(500)
        #a.set_path_cb(lv.anim_t.path_ease_out)
        #if forward:
        #    a.set_values(187, 0)
        #else:
        #    a.set_values(187, 374)
        #a.set_custom_exec_cb(lambda a,val: anim_x_cb(screen_img_album,val))
        #a.set_ready_cb(lv.obj.del_anim_ready_cb)
        #lv.anim_t.start(a)
        
        if next is None:
            if forward:
                next = (current + 1) % self.active_track_count
            else:
                next = (current - 1 + self.active_track_count) % self.active_track_count
            
        self.current_track_idx = next
        btn = self.btn[next]
        icon = self.icon[next]
        if demoMP.playing:
            btn.add_state(lv.STATE.PRESSED)
            icon.set_src(screen_img_1_img)
        btn.scroll_to_view(lv.ANIM.ON)
        screen_slider_1.set_range(0, self.time[next])
        screen_label_title_music.set_text(self.title[next])
        screen_label_title_author.set_text(self.author[next])
        #a = lv.anim_t()
        #a.init()
        #a.set_var(screen_img_album)
        #a.set_time(500)
        #a.set_path_cb(lv.anim_t.path_linear)
        #a.set_values(lv.IMG_ZOOM.NONE, lv.IMG_ZOOM.NONE // 2)
        #a.set_custom_exec_cb(lambda a,val: anim_zoom_cb(screen_img_album,val))
        #a.set_ready_cb(None)
        #lv.anim_t.start(a)
        #screen_img_album.set_src(demoMP.img_dsc[idx])
        screen_img_album_update_src(current, next)
        #screen_img_album.fade_in(500, 100)
        #a = lv.anim_t()
        #a.init()
        #a.set_var(screen_img_album)
        #a.set_time(500)
        #a.set_delay(100)
        #a.set_path_cb(lv.anim_t.path_overshoot)
        #a.set_values(lv.IMG_ZOOM.NONE // 4, lv.IMG_ZOOM.NONE)
        #a.set_custom_exec_cb(lambda a,val: anim_zoom_cb(screen_img_album,val))
        #a.set_ready_cb(None)
        #lv.anim_t.start(a)
        
demoMP = MusicPlayer()

def screen_slider_1_event_cb(e):
    slider = e.get_target()
    demoMP.slider_time = slider.get_value()

screen_slider_1.add_event_cb(screen_slider_1_event_cb, lv.EVENT.VALUE_CHANGED, None)

def set_slider_time_cb(timer):
    demoMP.slider_time += 1
    if demoMP.slider_time >= demoMP.time[demoMP.current_track_idx]:
        demoMP.load_next_track(forward=True)
        if demoMP.playing:
            demoMP.resume_track()
    else:
        screen_label_slider_time.set_text(f"{demoMP.slider_time // 60}:{demoMP.slider_time % 60:02}")
        screen_slider_1.set_value(demoMP.slider_time, lv.ANIM.ON)

sec_counter_timer = lv.timer_create(set_slider_time_cb, 1000, None)
sec_counter_timer.pause()

def screen_btn_tracks_clicked_2_event_cb(e,screen_player):
    screen_player_event_move_y = lv.anim_t()
    screen_player_event_move_y.init()
    screen_player_event_move_y.set_var(screen_player)
    if demoMP.show_track_list:
        screen_player_event_move_y.set_values(screen_player.get_y(), 0)
        demoMP.show_track_list = False
    else:
        screen_player_event_move_y.set_values(screen_player.get_y(), -261)
        demoMP.show_track_list = True
    screen_player_event_move_y.set_time(1000)
    screen_player_event_move_y.set_path_cb(lv.anim_t.path_linear)
    screen_player_event_move_y.set_custom_exec_cb(lambda a,val: anim_y_cb(screen_player,val))
    lv.anim_t.start(screen_player_event_move_y)
screen_btn_tracks.add_event_cb(lambda e: screen_btn_tracks_clicked_2_event_cb(e,screen_player), lv.EVENT.CLICKED, None)

def screen_img_album_gesture_event_cb(e):
    indev = lv.indev_get_act()
    if not indev:
        return
    dir = indev.get_gesture_dir()
    if dir == lv.DIR.LEFT:
        demoMP.load_next_track(forward=False)
    if dir == lv.DIR.RIGHT:
        demoMP.load_next_track(forward=True)
    if demoMP.playing:
        demoMP.resume_track()

screen_img_album.add_event_cb(lambda e: screen_img_album_gesture_event_cb(e), lv.EVENT.GESTURE, None)
screen_img_9.add_event_cb(lambda e: screen_img_album_gesture_event_cb(e), lv.EVENT.GESTURE, None)
screen_img_10.add_event_cb(lambda e: screen_img_album_gesture_event_cb(e), lv.EVENT.GESTURE, None)
screen_img_album.clear_flag(lv.obj.FLAG.GESTURE_BUBBLE)
screen_img_9.clear_flag(lv.obj.FLAG.GESTURE_BUBBLE)
screen_img_10.clear_flag(lv.obj.FLAG.GESTURE_BUBBLE)

def screen_imgbtn_play_released_event_cb(e,obj):
    code = e.get_code()
    btn = demoMP.btn[demoMP.current_track_idx]
    icon = demoMP.icon[demoMP.current_track_idx]
    if code == lv.EVENT.RELEASED:
        if obj.has_state(lv.STATE.CHECKED):
            demoMP.playing = True
            demoMP.resume_track()
            btn.add_state(lv.STATE.PRESSED)
            icon.set_src(screen_img_1_img)
        else:
            demoMP.playing = False
            demoMP.pause_track()
            btn.clear_state(lv.STATE.PRESSED)
            icon.set_src(screen_img_2_img)

screen_imgbtn_play.add_event_cb(lambda e: screen_imgbtn_play_released_event_cb(e,screen_imgbtn_play), lv.EVENT.ALL, None)

def screen_img_icn_clicked_event_cb(e,val):
    code = e.get_code()
    if code == lv.EVENT.CLICKED:
        demoMP.load_next_track(forward=val)
        if demoMP.playing:
            demoMP.resume_track()

screen_img_icn_left.add_event_cb(lambda e: screen_img_icn_clicked_event_cb(e,False), lv.EVENT.ALL, None)
screen_img_icn_right.add_event_cb(lambda e: screen_img_icn_clicked_event_cb(e,True), lv.EVENT.ALL, None)

screen_img_1.set_src(screen_img_2_img)
screen_slider_1.set_range(0, demoMP.time[0])
def screen_track_clicked_event_cb(e,idx):
    code = e.get_code()
    if code == lv.EVENT.CLICKED:
        btn = demoMP.btn[idx]
        icon = demoMP.icon[idx]
        if demoMP.current_track_idx == idx:
            if demoMP.playing:
                demoMP.playing = False
                demoMP.pause_track()
                btn.clear_state(lv.STATE.PRESSED)
                icon.set_src(screen_img_2_img)
            else:
                demoMP.playing = True
                demoMP.resume_track()
                btn.add_state(lv.STATE.PRESSED)
                icon.set_src(screen_img_1_img)
        else:
            demoMP.playing = True
            demoMP.load_next_track(next=idx)
            demoMP.resume_track()

screen_btn_1.add_event_cb(lambda e: screen_track_clicked_event_cb(e, 0), lv.EVENT.CLICKED, None)
screen_img_1.add_event_cb(lambda e: screen_track_clicked_event_cb(e, 0), lv.EVENT.CLICKED, None)
screen_btn_2.add_event_cb(lambda e: screen_track_clicked_event_cb(e, 1), lv.EVENT.CLICKED, None)
screen_img_2.add_event_cb(lambda e: screen_track_clicked_event_cb(e, 1), lv.EVENT.CLICKED, None)
screen_btn_3.add_event_cb(lambda e: screen_track_clicked_event_cb(e, 2), lv.EVENT.CLICKED, None)
screen_img_3.add_event_cb(lambda e: screen_track_clicked_event_cb(e, 2), lv.EVENT.CLICKED, None)

