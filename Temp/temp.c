/* 全局组件变量声明 */
lv_obj_t *screen;
lv_obj_t *title_label;
lv_obj_t *topology_canvas;
lv_obj_t *res_label;
lv_obj_t *ind_label;
lv_obj_t *cap_label;
lv_obj_t *status_bar;
lv_obj_t *conn_dot;
lv_obj_t *serial_label;

/* 初始化函数 */
void setup_scr_screen(lv_ui *ui) {
    // 设置屏幕背景色
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xF0F0F0), LV_PART_MAIN);

    /* 标题区域 */
    title_label = lv_label_create(screen);
    lv_obj_set_pos(title_label, 10, 5);
    lv_obj_set_size(title_label, 460, 40);
    lv_label_set_text(title_label, "RLC Meter - Serial Mode");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0x2A5CAA), 0);

    /* 拓扑图区域 */
    static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(200, 150)];
    topology_canvas = lv_canvas_create(screen);
    lv_canvas_set_buffer(topology_canvas, cbuf, 200, 150, LV_IMG_CF_TRUE_COLOR);
    lv_obj_set_pos(topology_canvas, 20, 60);
    lv_canvas_fill_bg(topology_canvas, lv_color_hex(0xFFFFFF), LV_OPA_COVER);

    // 绘制默认串联拓扑
    // lv_canvas_draw_line(topology_canvas, (lv_point_t[]){ {20,75}, {180,75} }, 2, lv_color_hex(0x000000), LV_OPA_COVER);
    lv_canvas_draw_rect(topology_canvas, 50, 60, 40, 30, lv_color_hex(0xC0C0C0), LV_OPA_COVER);
    lv_canvas_draw_text(topology_canvas, 60, 65, 40, &lv_font_montserrat_16, "R", LV_OPA_COVER);
    lv_canvas_draw_rect(topology_canvas, 100, 60, 40, 30, lv_color_hex(0xC0C0C0), LV_OPA_COVER);
    lv_canvas_draw_text(topology_canvas, 110, 65, 40, &lv_font_montserrat_16, "L", LV_OPA_COVER);
    lv_canvas_draw_rect(topology_canvas, 150, 60, 40, 30, lv_color_hex(0xC0C0C0), LV_OPA_COVER);
    lv_canvas_draw_text(topology_canvas, 160, 65, 40, &lv_font_montserrat_16, "C", LV_OPA_COVER);

    /* 测量值显示区域 */
    lv_obj_t *meas_panel = lv_cont_create(screen);
    lv_obj_set_pos(meas_panel, 260, 60);
    lv_obj_set_size(meas_panel, 200, 200);
    lv_obj_set_style_bg_color(meas_panel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_pad_all(meas_panel, 10, 0);

    res_label = lv_label_create(meas_panel);
    lv_obj_set_pos(res_label, 0, 0);
    lv_label_set_text(res_label, "Resistance:\n0.00 Ω");
    lv_obj_set_style_text_font(res_label, &lv_font_montserrat_18, 0);

    ind_label = lv_label_create(meas_panel);
    lv_obj_set_pos(ind_label, 0, 70);
    lv_label_set_text(ind_label, "Inductance:\n0.00 mH");
    lv_obj_set_style_text_font(ind_label, &lv_font_montserrat_18, 0);

    cap_label = lv_label_create(meas_panel);
    lv_obj_set_pos(cap_label, 0, 140);
    lv_label_set_text(cap_label, "Capacitance:\n0.00 uF");
    lv_obj_set_style_text_font(cap_label, &lv_font_montserrat_18, 0);

    /* 状态栏 */
    status_bar = lv_cont_create(screen);
    lv_obj_set_pos(status_bar, 0, 280);
    lv_obj_set_size(status_bar, 480, 40);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(0xE0E0E0), 0);

    conn_dot = lv_led_create(status_bar);
    lv_obj_set_pos(conn_dot, 10, 10);
    lv_led_set_color(conn_dot, lv_color_hex(0x00FF00));
    lv_led_off(conn_dot);

    lv_obj_t *conn_label = lv_label_create(status_bar);
    lv_obj_set_pos(conn_label, 40, 12);
    lv_label_set_text(conn_label, "Connected");

    serial_label = lv_label_create(status_bar);
    lv_obj_set_pos(serial_label, 360, 12);
    lv_label_set_text(serial_label, "COM: --");
    lv_obj_set_style_text_font(serial_label, &lv_font_montserrat_16, 0);
}

/* API函数部分 */
void update_measurements(float res, float ind, float cap) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "Resistance:\n%.2f Ω", res);
    lv_label_set_text(res_label, buf);

    snprintf(buf, sizeof(buf), "Inductance:\n%.2f mH", ind);
    lv_label_set_text(ind_label, buf);

    snprintf(buf, sizeof(buf), "Capacitance:\n%.2f uF", cap);
    lv_label_set_text(cap_label, buf);
}

void set_topology(uint8_t type) {
    lv_canvas_fill_bg(topology_canvas, lv_color_hex(0xFFFFFF), LV_OPA_COVER);

    if(type == 0) { // Serial
        lv_label_set_text(title_label, "RLC Meter - Serial Mode");
        // 绘制串联拓扑图形...
    } else { // Parallel
        lv_label_set_text(title_label, "RLC Meter - Parallel Mode");
        // 绘制并联拓扑图形...
    }
}

void set_connection_status(bool connected, const char *port) {
    if(connected) {
        lv_led_on(conn_dot);
        static char buf[16];
        snprintf(buf, sizeof(buf), "COM: %s", port);
        lv_label_set_text(serial_label, buf);
    } else {
        lv_led_off(conn_dot);
        lv_label_set_text(serial_label, "COM: --");
    }
}