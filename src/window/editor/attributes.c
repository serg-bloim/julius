#include "attributes.h"

#include "core/image.h"
#include "core/image_group_editor.h"
#include "game/resource.h"
#include "graphics/arrow_button.h"
#include "graphics/button.h"
#include "graphics/generic_button.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "input/keyboard.h"
#include "scenario/editor.h"
#include "scenario/property.h"
#include "window/editor/allowed_buildings.h"
#include "window/editor/invasions.h"
#include "window/editor/map.h"
#include "window/editor/price_changes.h"
#include "window/editor/requests.h"
#include "window/editor/special_events.h"
#include "window/editor/starting_conditions.h"
#include "window/editor/win_criteria.h"
#include "window/select_list.h"

static void button_click(int p1, int p2) {}

static void button_starting_conditions(int param1, int param2);
static void button_requests(int param1, int param2);
static void button_enemy(int param1, int param2);
static void button_invasions(int param1, int param2);
static void button_allowed_buildings(int param1, int param2);
static void button_win_criteria(int param1, int param2);
static void button_special_events(int param1, int param2);
static void button_price_changes(int param1, int param2);
static void change_climate(int param1, int param2);
static void change_image(int forward, int param2);

static generic_button buttons[] = {
    {212, 76, 462, 106, GB_IMMEDIATE, button_starting_conditions, button_none, 1, 0},
    {212, 116, 462, 146, GB_IMMEDIATE, change_climate, button_none, 2, 0},
    {212, 156, 462, 186, GB_IMMEDIATE, button_requests, button_none, 3, 0},
    {212, 196, 462, 226, GB_IMMEDIATE, button_enemy, button_none, 4, 0},
    {212, 236, 462, 266, GB_IMMEDIATE, button_invasions, button_none, 5, 0},
    {212, 276, 462, 306, GB_IMMEDIATE, button_allowed_buildings, button_none, 6, 0},
    {212, 316, 462, 346, GB_IMMEDIATE, button_win_criteria, button_none, 7, 0},
    {212, 356, 462, 386, GB_IMMEDIATE, button_special_events, button_none, 8, 0},
    {212, 396, 462, 426, GB_IMMEDIATE, button_price_changes, button_none, 9, 0},
    {212, 436, 462, 466, GB_IMMEDIATE, button_click, button_none, 10, 0},
};

static arrow_button image_arrows[] = {
    {20, 424, 19, 24, change_image, 0, 0},
    {44, 424, 21, 24, change_image, 1, 0},
};

static int focus_button_id;

static void draw_background(void)
{
    graphics_clear_screen();
}

static void draw_foreground(void)
{
    graphics_in_dialog();
    outer_panel_draw(0, 28, 30, 28);

    inner_panel_draw(109, 40, 18, 2);
    //text_capture_cursor(keyboard_cursor_position(), keyboard_offset_start(), keyboard_offset_end());
    //text_draw_centered(TEXT, 112, 50, 280, FONT_NORMAL_WHITE, 0);
    //text_draw_cursor(212, 76, keyboard_is_insert());

    button_border_draw(212, 76, 250, 30, focus_button_id == 1);
    lang_text_draw_centered(44, 88, 212, 85, 250, FONT_NORMAL_BLACK);

    lang_text_draw(44, 76, 32, 125, FONT_NORMAL_BLACK);
    button_border_draw(212, 116, 250, 30, focus_button_id == 2);
    lang_text_draw_centered(44, 77 + scenario_property_climate(), 212, 125, 250, FONT_NORMAL_BLACK);

    lang_text_draw(44, 40, 32, 165, FONT_NORMAL_BLACK);
    button_border_draw(212, 156, 250, 30, focus_button_id == 3);

    editor_request request;
    scenario_editor_request_get(0, &request);
    if (request.resource) {
        lang_text_draw_year(scenario_property_start_year() + request.year, 222, 165, FONT_NORMAL_BLACK);
        int width = text_draw_number(request.amount, '@', " ", 312, 165, FONT_NORMAL_BLACK);
        int offset = request.resource + resource_image_offset(request.resource, RESOURCE_IMAGE_ICON);
        image_draw(image_group(GROUP_EDITOR_RESOURCE_ICONS) + offset, 322 + width, 160);
    } else {
        lang_text_draw_centered(44, 19, 212, 165, 250, FONT_NORMAL_BLACK);
    }

    lang_text_draw(44, 41, 32, 205, FONT_NORMAL_BLACK);
    button_border_draw(212, 196, 250, 30, focus_button_id == 4);
    lang_text_draw_centered(37, scenario_property_enemy(), 212, 205, 250, FONT_NORMAL_BLACK);

    lang_text_draw(44, 42, 32, 245, FONT_NORMAL_BLACK);
    button_border_draw(212, 236, 250, 30, focus_button_id == 5);
    
    editor_invasion invasion;
    scenario_editor_invasion_get(0, &invasion);
    if (invasion.type) {
        lang_text_draw_year(scenario_property_start_year() + invasion.year, 222, 245, FONT_NORMAL_BLACK);
        int width = text_draw_number(invasion.amount, '@', " ", 302, 245, FONT_NORMAL_BLACK);
        lang_text_draw(34, invasion.type, 302 + width, 245, FONT_NORMAL_BLACK);
    } else {
        lang_text_draw_centered(44, 20, 212, 245, 250, FONT_NORMAL_BLACK);
    }

    button_border_draw(212, 276, 250, 30, focus_button_id == 6);
    lang_text_draw_centered(44, 44, 212, 285, 250, FONT_NORMAL_BLACK);

    button_border_draw(212, 316, 250, 30, focus_button_id == 7);
    lang_text_draw_centered(44, 45, 212, 325, 250, FONT_NORMAL_BLACK);

    button_border_draw(212, 356, 250, 30, focus_button_id == 8);
    lang_text_draw_centered(44, 49, 212, 365, 250, FONT_NORMAL_BLACK);

    button_border_draw(212, 396, 250, 30, focus_button_id == 9);
    lang_text_draw_centered(44, 95, 212, 405, 250, FONT_NORMAL_BLACK);

    button_border_draw(212, 436, 250, 30, focus_button_id == 10);
    lang_text_draw_centered(44, 94, 212, 445, 250, FONT_NORMAL_BLACK);

    button_border_draw(18, 278, 184, 144, 0);
    image_draw(image_group(GROUP_EDITOR_SCENARIO_IMAGE) + scenario_image_id(), 20, 280);

    arrow_buttons_draw(0, 0, image_arrows, 2);

    graphics_reset_dialog();
}

static void handle_mouse(const mouse *m)
{
    if (m->right.went_down) {
        window_editor_map_show();
    }
    const mouse *m_dialog = mouse_in_dialog(m);
    if (!generic_buttons_handle_mouse(m_dialog, 0, 0, buttons, 10, &focus_button_id)) {
        arrow_buttons_handle_mouse(m_dialog, 0, 0, image_arrows, 2);
    }
}

static void button_starting_conditions(int param1, int param2)
{
    window_editor_starting_conditions_show();
}

static void button_requests(int param1, int param2)
{
    window_editor_requests_show();
}

static void button_enemy(int param1, int param2)
{
    window_select_list_show(20, 40, 20, 37, scenario_editor_set_enemy);
}

static void button_invasions(int param1, int param2)
{
    window_editor_invasions_show();
}

static void button_allowed_buildings(int param1, int param2)
{
    window_editor_allowed_buildings_show();
}

static void button_win_criteria(int param1, int param2)
{
    window_editor_win_criteria_show();
}

static void button_special_events(int param1, int param2)
{
    window_editor_special_events_show();
}

static void button_price_changes(int param1, int param2)
{
    window_editor_price_changes_show();
}

static void change_climate(int param1, int param2)
{
    scenario_editor_cycle_climate();
    image_load_climate(scenario_property_climate(), 1);
    window_request_refresh();
}

static void change_image(int forward, int param2)
{
    scenario_editor_cycle_image(forward);
    window_request_refresh();
}

void window_editor_attributes_show(void)
{
    window_type window = {
        WINDOW_EDITOR_ATTRIBUTES,
        draw_background,
        draw_foreground,
        handle_mouse
    };
    window_show(&window);
}
