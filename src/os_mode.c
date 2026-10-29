/*
 * Bluetoothの接続先(プロファイル)が切り替わったとき、
 * 決めておいたプロファイルならMac/Winモード(MACレイヤーのON/OFF)を合わせる。
 * それ以外のプロファイルではモードを変えない。
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/keymap.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int os_mode_listener(const zmk_event_t *eh) {
    const struct zmk_ble_active_profile_changed *ev = as_zmk_ble_active_profile_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    zmk_keymap_layer_id_t mac_layer = zmk_keymap_layer_index_to_id(CONFIG_GLOVE43TB_MAC_LAYER);

    if (ev->index == CONFIG_GLOVE43TB_MAC_BT_PROFILE) {
        LOG_DBG("profile %d: Mac mode", ev->index);
        zmk_keymap_layer_activate(mac_layer);
    } else if (ev->index == CONFIG_GLOVE43TB_WIN_BT_PROFILE) {
        LOG_DBG("profile %d: Win mode", ev->index);
        zmk_keymap_layer_deactivate(mac_layer);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(os_mode, os_mode_listener);
ZMK_SUBSCRIPTION(os_mode, zmk_ble_active_profile_changed);
