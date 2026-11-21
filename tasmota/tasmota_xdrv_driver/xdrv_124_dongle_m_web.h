/*
 * xdrv_124_dongle_m_web.h - Web UI resources for SONOFF Dongle Max
 *
 * Copyright (C) SONOFF
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef _XDRV_124_DONGLE_M_WEB_H_
#define _XDRV_124_DONGLE_M_WEB_H_

// -----------------------------------------------------------------------------
// Styles
// -----------------------------------------------------------------------------
static const char DONGLEM_WEB_STYLE[] PROGMEM = R"=====(
<style>
  #dm {
    --line: color-mix(in srgb, var(--c_txt) 22%, transparent);
    --muted: color-mix(in srgb, var(--c_txt) 68%, transparent);
    --label-column: minmax(165px, 42%);
    width: min(460px, calc(100vw - 24px));
    margin: 8px auto 22px;
    color: var(--c_txt);
    text-align: left;
  }

  #dm *,
  #dm *:before,
  #dm *:after {
    box-sizing: border-box;
  }

  #dm .theme {
    position: relative;
    display: block;
    width: 86px;
    height: 36px;
    margin: 0 0 14px;
    padding: 3px;
    border: 1px solid var(--line);
    border-radius: 999px;
    background: var(--c_frm);
    color: var(--c_txt);
    font-size: .78rem;
    line-height: 28px;
    overflow: hidden;
  }

  #dm .theme-icon {
    display: grid;
    place-items: center;
    width: 28px;
    height: 28px;
    border-radius: 50%;
    background: var(--c_btn);
    color: #fff;
    font-size: .95rem;
    position: absolute;
    top: 3px;
    left: 3px;
    transition: transform .22s ease;
  }

  #dm .theme-label {
    position: absolute;
    top: 3px;
    right: 9px;
    transition: left .22s ease, right .22s ease;
  }

  #dm .theme.light .theme-icon {
    transform: translateX(48px);
  }

  #dm .theme.light .theme-label {
    left: 9px;
    right: auto;
  }

  #dm.initializing .theme-icon,
  #dm.initializing .theme-label,
  #dm.initializing .switch-track,
  #dm.initializing .switch-track:after,
  #dm.theme-changing .switch-track,
  #dm.theme-changing .switch-track:after {
    transition: none !important;
  }

  #dm .panel {
    padding: 6px 15px 15px;
    background: var(--c_frm);
    border: 1px solid var(--c_btn);
    border-radius: 12px;
  }

  #dm .row {
    display: grid;
    grid-template-columns: var(--label-column) minmax(0, 1fr);
    align-items: center;
    column-gap: 12px;
    min-height: 45px;
    padding: 6px 0;
  }

  #dm .row > :last-child {
    justify-self: end;
  }

  #dm .row > span:first-child,
  #dm .label {
    font-size: .9rem;
    color: var(--c_txt);
  }

  #dm .value {
    max-width: 100%;
    text-align: right;
    font-size: .9rem;
    overflow-wrap: anywhere;
  }

  #dm .ok {
    color: var(--c_txtscc);
  }

  #dm .bad {
    color: var(--c_txtwrn);
  }

  #dm .hidden {
    display: none !important;
  }

  #dm .bars {
    display: inline-flex;
    align-items: flex-end;
    gap: 2px;
    height: 16px;
    margin-left: 7px;
  }

  #dm .bars i {
    display: block;
    width: 3px;
    background: var(--muted);
    opacity: .38;
  }

  #dm .bars i:nth-child(1) { height: 4px; }
  #dm .bars i:nth-child(2) { height: 7px; }
  #dm .bars i:nth-child(3) { height: 11px; }
  #dm .bars i:nth-child(4) { height: 15px; }

  #dm .bars.q1 i:nth-child(-n+1),
  #dm .bars.q2 i:nth-child(-n+2),
  #dm .bars.q3 i:nth-child(-n+3),
  #dm .bars.q4 i:nth-child(-n+4) {
    background: var(--c_txtscc);
    opacity: 1;
  }

  #dm .field {
    padding: 10px 0;
  }

  #dm .field.compact {
    display: grid;
    grid-template-columns: var(--label-column) minmax(0, 1fr);
    align-items: center;
    column-gap: 12px;
    min-height: 45px;
    padding: 6px 0;
  }

  #dm .field.compact > input:not([type=checkbox]):not([type=range]),
  #dm .field.compact > select {
    grid-column: 2;
    width: 100%;
  }

  #dm .field-head {
    display: flex;
    align-items: center;
    justify-content: space-between;
    gap: 12px;
    margin-bottom: 7px;
  }

  #dm .note {
    margin: 4px 0 7px;
    font-size: .72rem;
    line-height: 1.45;
    color: var(--muted);
  }

  #dm input:not([type=checkbox]):not([type=range]),
  #dm select {
    width: 100%;
    height: 42px;
    margin: 3px 0 8px;
    padding: 8px 10px;
    border: 1px solid var(--line);
    border-radius: 7px;
    background: var(--c_in);
    color: var(--c_intxt);
  }

  #dm input[type=range] {
    width: 100%;
    height: 24px;
    margin: 0;
    padding: 0;
    accent-color: var(--c_btn);
  }

  #dm .color-range {
    appearance: none;
    -webkit-appearance: none;
    height: 9px !important;
    margin: 8px 0 13px !important;
    border-radius: 999px;
    background: linear-gradient(90deg, #f00, #ff0, #0f0, #0ff, #00f, #f0f, #f00);
    outline: none;
  }

  #dm .color-range::-webkit-slider-thumb {
    -webkit-appearance: none;
    width: 22px;
    height: 22px;
    border: 3px solid #fff;
    border-radius: 50%;
    background: var(--c_btn);
    box-shadow: 0 1px 6px rgba(0, 0, 0, .5);
  }

  #dm .color-range::-moz-range-thumb {
    width: 17px;
    height: 17px;
    border: 3px solid #fff;
    border-radius: 50%;
    background: var(--c_btn);
  }

  #dm .color-value {
    display: flex;
    align-items: center;
    gap: 8px;
  }

  #dm .swatch {
    width: 25px;
    height: 25px;
    border: 2px solid color-mix(in srgb, var(--c_txt) 55%, transparent);
    border-radius: 5px;
    background: #0000ff;
  }

  #dm .switch {
    position: relative;
    display: inline-flex;
    align-items: center;
    cursor: pointer;
  }

  #dm .switch input {
    position: absolute;
    opacity: 0;
    pointer-events: none;
  }

  #dm .switch-track {
    position: relative;
    width: 48px;
    height: 27px;
    border-radius: 999px;
    background: color-mix(in srgb, var(--c_txt) 34%, var(--c_bg));
    transition: .2s;
  }

  #dm .switch-track:after {
    content: '';
    position: absolute;
    width: 21px;
    height: 21px;
    left: 3px;
    top: 3px;
    border-radius: 50%;
    background: #fff;
    box-shadow: 0 2px 6px rgba(0, 0, 0, .3);
    transition: .2s;
  }

  #dm .switch input:checked + .switch-track {
    background: var(--c_btn);
  }

  #dm .switch input:checked + .switch-track:after {
    transform: translateX(21px);
  }

  #dm button.small {
    flex: 0 0 auto;
    width: auto;
    height: 38px;
    margin: 0;
    padding: 0 15px;
    font-size: .9rem;
    line-height: 1;
  }

  #dm .field.compact input:not([type=checkbox]):not([type=range]),
  #dm .field.compact select {
    height: 38px;
    margin: 0;
  }

  #dm .danger {
    background: var(--c_btnrst) !important;
  }

  #dm .mode {
    display: flex;
    align-items: center;
    gap: 9px;
  }

  /* Show APP / BOOT inside the MG24 switch, without affecting Light. */
  #dm .mode-switch .switch-track {
    width: 78px;
    height: 28px;
  }

  #dm .mode-switch .switch-track::before {
    content: 'APP';
    position: absolute;
    top: 0;
    right: 9px;
    display: flex;
    align-items: center;
    height: 100%;
    font-size: .72rem;
    font-weight: 700;
    line-height: 1;
    color: var(--c_txt);
    pointer-events: none;
  }

  #dm .mode-switch input:checked + .switch-track::before {
    content: 'BOOT';
    left: 8px;
    right: auto;
    color: var(--c_btntxt);
  }

  #dm .mode-switch input:checked + .switch-track::after {
    transform: translateX(51px);
  }

  #dm dialog {
    width: min(390px, calc(100vw - 32px));
    padding: 20px;
    border: 1px solid var(--line);
    border-radius: 12px;
    background: var(--c_frm);
    color: var(--c_txt);
  }

  #dm dialog::backdrop {
    background: rgba(0, 0, 0, .58);
  }

  #dm dialog h3 {
    margin: 0 0 10px;
    color: var(--c_txt);
  }

  #dm .dialog-actions {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 9px;
    margin-top: 18px;
  }


  #dm .dialog-actions button {
    display: flex;
    align-items: center;
    justify-content: center;

    width: 100%;
    height: 44px;
    min-height: 44px;
    margin: 0 !important;
    padding: 0 12px;

    box-sizing: border-box;
    border: 0;
    border-radius: 6px;

    font-size: 1rem;
    line-height: 1;
  }


  #dm .cancel {
    background: var(--c_btnoff);
    color: var(--c_txt);
    margin-top: 6px;
  }

  @media (max-width: 400px) {
    #dm {
      --label-column: minmax(120px, 42%);
    }

    #dm .panel {
      padding-right: 12px;
      padding-left: 12px;
    }

    #dm button.small {
      padding-right: 10px;
      padding-left: 10px;
    }
  }

  /* Right-align compact input groups */
  #dm .inline {
    display: flex;
    align-items: center;
    justify-content: flex-end;
    gap: 8px;
  }

  /* Prevent input from filling the entire row */
  #dm .inline input {
    flex: 0 1 auto;
    width: 130px;
    min-width: 0;
    margin-left: auto;
  }

  /* Align compact controls to the right */
  #dm .field.compact > input:not([type=checkbox]):not([type=range]),
  #dm .field.compact > select {
    grid-column: 2;
    justify-self: end;
  }

  /* Hostname: compact and auto-expanding */
  #dm #dm-hostname {
    min-width: 108px;
    max-width: 100%;
    white-space: nowrap;
  }

  /* TCP Port */
  #dm #dm-port {
    width: 108px;
    min-width: 0;
    max-width: 100%;
  }

  /* Baud Rate: right-aligned select */
  #dm #dm-baud {
    display: block;
    width: 108px;
    max-width: 100%;
    margin: 3px 0 8px auto;
  }


</style>
)=====";

// -----------------------------------------------------------------------------
// Markup
// -----------------------------------------------------------------------------
static const char DONGLEM_WEB_BODY[] PROGMEM = R"=====(
<section id="dm" class="initializing">
  <button id="dm-theme" class="theme" type="button" aria-label="Switch color theme">
    <span id="dm-theme-icon" class="theme-icon">☾</span>
    <span id="dm-theme-label" class="theme-label">Dark</span>
  </button>

  <div class="panel">
    <div id="dm-eth-down" class="row">
      <span>Ethernet Status</span>
      <b class="value bad">Disconnected</b>
    </div>

    <div id="dm-eth-up" class="row hidden">
      <span>Ethernet IP</span>
      <b id="dm-eth-ip" class="value"></b>
    </div>

    <div id="dm-wifi-down" class="row">
      <span>Wi-Fi Status</span>
      <b class="value bad">Disconnected</b>
    </div>

    <div id="dm-wifi-ip-row" class="row hidden">
      <span>Wi-Fi IP</span>
      <b id="dm-wifi-ip" class="value"></b>
    </div>

    <div id="dm-wifi-rssi-row" class="row hidden">
      <span>Wi-Fi RSSI</span>
      <span class="value">
        <b id="dm-wifi-rssi"></b>
        <span id="dm-bars" class="bars" aria-hidden="true">
          <i></i><i></i><i></i><i></i>
        </span>
      </span>
    </div>

    <div class="row">
      <span>UART over TCP</span>
      <b id="dm-tcp-state" class="value">Disconnected</b>
    </div>

    <div class="row">
      <span>Button Event</span>
      <b id="dm-button" class="value">-</b>
    </div>

    <div class="field compact">
      <label class="label" for="dm-hostname">Hostname</label>
      <input id="dm-hostname" maxlength="38" autocomplete="off">
    </div>

    <div class="row">
      <span>Light</span>
      <label class="switch" aria-label="Light power">
        <input id="dm-light-power" type="checkbox">
        <span class="switch-track"></span>
      </label>
    </div>

    <div id="dm-light-controls">
      <div class="field">
        <div class="field-head">
          <label class="label" for="dm-brightness">Brightness</label>
          <b id="dm-brightness-value">75%</b>
        </div>
        <input id="dm-brightness" type="range" min="1" max="100" value="75">
      </div>

      <div class="field">
        <div class="field-head">
          <label class="label" for="dm-hue">Color</label>
          <span class="color-value">
            <b id="dm-color-value">#0000FF</b>
            <i id="dm-swatch" class="swatch"></i>
          </span>
        </div>
        <input id="dm-hue" class="color-range" type="range" min="0" max="359" value="240">
      </div>
    </div>

    <div class="field compact">
      <label class="label" for="dm-port">TCP Port</label>
      <input id="dm-port" type="number" min="1" max="65535">
    </div>

    <div class="row">
      <span>MG24 Restart</span>
      <button id="dm-restart" class="small danger" type="button">Restart</button>
    </div>

    <div class="row">
      <span>MG24 Bootloader Mode</span>
      <div class="mode">
        <label class="switch mode-switch">
          <input id="dm-boot" type="checkbox" aria-label="MG24 bootloader mode">
          <span class="switch-track"></span>
        </label>
      </div>
    </div>

    <div class="field compact">
      <label class="label" for="dm-baud">MG24 Baud Rate</label>
      <select id="dm-baud">
        <option>115200</option>
        <option>460800</option>
        <option>921600</option>
      </select>
    </div>
  </div>

  <dialog id="dm-restart-dialog">
    <h3>Restart MG24</h3>
    <p>Are you sure you want to restart the MG24 SoC?</p>
    <div class="dialog-actions">
      <button id="dm-restart-cancel" class="cancel" type="button">Cancel</button>
      <button id="dm-restart-confirm" class="danger" type="button">Restart</button>
    </div>
  </dialog>
</section>
)=====";

// -----------------------------------------------------------------------------
// Behavior
// -----------------------------------------------------------------------------
static const char DONGLEM_WEB_SCRIPT[] PROGMEM = R"=====(
<script>
  (() => {
    'use strict';

    const $ = id => document.getElementById(id);

    const state = {
      polling: false,
      saving: false,
      lightAvailable: false,
      theme: null,
      initialized: false,
      dirty: new Set()
    };

    const text = (id, value) => {
      $(id).textContent = value == null || value === '' ? '-' : String(value);
    };

    const hidden = (id, value) => {
      $(id).classList.toggle('hidden', !!value);
    };

    const editing = id => document.activeElement === $(id);

    const sync = (id, value, group) => {
      if (!state.dirty.has(group) && !editing(id)) {
        $(id).value = String(value ?? '');
      }
    };

    function resizeHostname() {
      const input = $('dm-hostname');

      // Minimum 12 characters, expand when longer
      const chars = Math.max(12, input.value.length + 2);

      input.style.width = chars + 'ch';
    }

    const hex = number => Math.round(number).toString(16).padStart(2, '0');

    const hueToHex = hue => {
      const f = (n, k = (n + hue / 60) % 6) => 255 * (1 - Math.max(Math.min(k, 4 - k, 1), 0));
      return ('#' + hex(f(5)) + hex(f(3)) + hex(f(1))).toUpperCase();
    };

    const hexToHue = value => {
      const number = parseInt(value, 16);
      const r = (number >> 16) / 255;
      const g = ((number >> 8) & 255) / 255;
      const b = (number & 255) / 255;
      const max = Math.max(r, g, b);
      const min = Math.min(r, g, b);
      const delta = max - min;

      if (!delta) return 0;

      let hue = max === r
        ? ((g - b) / delta) % 6
        : max === g
          ? (b - r) / delta + 2
          : (r - g) / delta + 4;

      return Math.round((hue * 60 + 360) % 360);
    };

    const showHue = hue => {
      const color = hueToHex(Number(hue));
      text('dm-color-value', color);
      $('dm-swatch').style.background = color;
      return color.substring(1);
    };

    const themeColors = {
      dark: [
        '#111722', '#1b2432', '#f4f7fb', '#e8edf5', '#ff6577',
        '#52c77a', '#268fd0', '#20334a', '#fff', '#1674ad',
        '#d94a4a', '#a42f2f', '#36a866', '#288451', '#101722',
        '#e8edf5', '#0b1018', '#72d572', '#657184', '#fff'
      ],
      light: [
        '#fff', '#f2f2f2', '#111827', '#000', '#d21f3c',
        '#087f23', '#147cc1', '#d8ecf8', '#fff', '#0e5f91',
        '#c62828', '#8e1d1d', '#2e9d52', '#237f41', '#fff',
        '#000', '#fff', '#000', '#777', '#fff'
      ]
    };

    const themeVariables = [
      '--c_bg', '--c_frm', '--c_ttl', '--c_txt', '--c_txtwrn',
      '--c_txtscc', '--c_btn', '--c_btnoff', '--c_btntxt', '--c_btnhvr',
      '--c_btnrst', '--c_btnrsthvr', '--c_btnsv', '--c_btnsvhvr', '--c_in',
      '--c_intxt', '--c_csl', '--c_csltxt', '--c_tab', '--c_tabtxt'
    ];

    const applyTheme = theme => {
      const selected = theme === 'light' ? 'light' : 'dark';
      if (state.theme === selected) return;

      const suppressSwitchAnimation = state.initialized;
      if (suppressSwitchAnimation) $('dm').classList.add('theme-changing');

      state.theme = selected;
      text('dm-theme-icon', selected === 'dark' ? '☾' : '☀');
      text('dm-theme-label', selected === 'dark' ? 'Dark' : 'Light');
      $('dm-theme').classList.toggle('light', selected === 'light');
      themeVariables.forEach((name, index) => {
        document.documentElement.style.setProperty(name, themeColors[selected][index]);
      });

      if (suppressSwitchAnimation) {
        requestAnimationFrame(() => requestAnimationFrame(() => {
          $('dm').classList.remove('theme-changing');
        }));
      }
    };

    const setAvailability = () => {
      ['dm-light-power', 'dm-brightness', 'dm-hue'].forEach(id => {
        $(id).disabled = !state.lightAvailable;
      });
    };

    async function post(url, data) {
      const response = await fetch(url, {
        method: 'POST',
        credentials: 'same-origin',
        cache: 'no-store',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: new URLSearchParams(data)
      });

      let result;
      try {
        result = await response.json();
      } catch (error) {
        throw Error('Invalid server response (' + response.status + ')');
      }

      if (!response.ok || result.ok !== true) {
        throw Error(result.message || 'Request failed (' + response.status + ')');
      }

      return result;
    }

    function render(data) {
      const ethernet = data.Ethernet;
      const wifi = data.Wifi;
      const tcp = data.TCP;
      const light = data.Light;
      const mg24 = data.MG24;
      const network = data.Network;

      // Ethernet / Wi-Fi
      hidden('dm-eth-down', ethernet.Connected);
      hidden('dm-eth-up', !ethernet.Connected);
      text('dm-eth-ip', ethernet.IP);

      hidden('dm-wifi-down', wifi.Connected);
      hidden('dm-wifi-ip-row', !wifi.Connected);
      hidden('dm-wifi-rssi-row', !wifi.Connected);
      text('dm-wifi-ip', wifi.IP);
      text('dm-wifi-rssi', wifi.RSSI + ' dBm');

      const quality = wifi.RSSI >= -55 ? 4 : wifi.RSSI >= -67 ? 3 : wifi.RSSI >= -75 ? 2 : 1;
      $('dm-bars').className = 'bars q' + quality;

      // TCP / button
      text('dm-tcp-state', tcp.Connected ? 'Connected' : 'Disconnected');
      $('dm-tcp-state').className = 'value ' + (tcp.Connected ? 'ok' : 'bad');
      text('dm-button', data.Button.Event
          ? data.Button.Time + ' (' + data.Button.Event.replace(/_/g, ' ') + ')'
          : '-'
      );

      // Network hostname
      sync('dm-hostname', network.Hostname, 'hostname');
      resizeHostname();

      // Light
      state.lightAvailable = !!light.Available;
      setAvailability();

      if (!editing('dm-light-power')) {
        $('dm-light-power').checked = !!light.Power;
      }
      hidden('dm-light-controls', !light.Power);

      if (!editing('dm-brightness')) {
        $('dm-brightness').value = light.Brightness;
      }
      text('dm-brightness-value', $('dm-brightness').value + '%');

      if (!editing('dm-hue')) {
        $('dm-hue').value = hexToHue(light.Color);
      }
      showHue($('dm-hue').value);

      // MG24 settings
      sync('dm-port', tcp.Port, 'port');

      if (!editing('dm-boot')) {
        const displayedMode = mg24.Busy ? mg24.RequestedMode : mg24.Mode;
        $('dm-boot').checked = displayedMode === 'BOOT';
      }
      sync('dm-baud', mg24.AppBaudrate, 'baud');
      $('dm-boot').disabled = state.saving || mg24.Busy;
      $('dm-restart').disabled = state.saving || mg24.Busy;

      // Color theme
      applyTheme(data.Theme || 'dark');

      if (!state.initialized) {
        state.initialized = true;
        requestAnimationFrame(() => $('dm').classList.remove('initializing'));
      }
    }

    async function refresh(force = false) {
      if (state.polling || (!force && state.saving) || document.hidden) return;

      state.polling = true;
      try {
        const response = await fetch('/donglem/status', {
          credentials: 'same-origin',
          cache: 'no-store'
        });

        if (!response.ok) {
          throw Error('Status unavailable (' + response.status + ')');
        }

        const result = await response.json();
        if (!result.DongleM) throw Error('Invalid Dongle-M status');
        render(result.DongleM);
      } catch (error) {
        console.error(error);
      } finally {
        state.polling = false;
      }
    }

    async function save(url, payload, group = null) {
      if (state.saving) return false;

      state.saving = true;
      let ok = false;

      try {
        await post(url, payload);
        ok = true;

        if (group) state.dirty.delete(group);
      } catch (error) {
        console.error(error);
      } finally {
        state.saving = false;
      }

      if (!ok) {
        if (group) state.dirty.delete(group);
        await refresh(true);
      }

      return ok;
    }

    // Theme / hostname
    $('dm-theme').onclick = async () => {
      if (state.saving) return;
      const next = state.theme === 'dark' ? 'light' : 'dark';
      applyTheme(next);
      $('dm-theme').disabled = true;
      try {
        await save('/donglem/action', {
          action: 'theme',
          value: next
        });
      } finally {
        $('dm-theme').disabled = false;
      }
    };

    $('dm-hostname').oninput = () => {
      state.dirty.add('hostname');
      resizeHostname();
    };

    $('dm-hostname').onblur = () => {
      if (state.dirty.has('hostname')) {
        save('/donglem/network', { hostname: $('dm-hostname').value }, 'hostname');
      }
    };
    $('dm-hostname').onkeydown = event => {
      if (event.key === 'Enter') event.currentTarget.blur();
    };

    // Light
    $('dm-light-power').onchange = () => {
      hidden('dm-light-controls', !$('dm-light-power').checked);
      save('/donglem/action', {
        action: 'light',
        power: $('dm-light-power').checked ? 1 : 0
      });
    };

    $('dm-brightness').oninput = () => {
      text('dm-brightness-value', $('dm-brightness').value + '%');
    };

    $('dm-brightness').onchange = () => save('/donglem/action', {
      action: 'light',
      brightness: $('dm-brightness').value
    });

    $('dm-hue').oninput = () => showHue($('dm-hue').value);
    $('dm-hue').onchange = () => save('/donglem/action', {
      action: 'light',
      color: showHue($('dm-hue').value)
    });

    // TCP / MG24
    $('dm-port').oninput = () => state.dirty.add('port');
    $('dm-port').onblur = () => {
      if (state.dirty.has('port')) {
        save('/donglem/action', { action: 'port', value: $('dm-port').value }, 'port');
      }
    };
    $('dm-port').onkeydown = event => {
      if (event.key === 'Enter') event.currentTarget.blur();
    };

    $('dm-boot').onchange = () => {
      const mode = $('dm-boot').checked ? 'BOOT' : 'APP';
      save('/donglem/action', { action: 'mode', mode });
    };

    $('dm-baud').onchange = () => save('/donglem/action', { action: 'baudrate', value: $('dm-baud').value }, 'baud'
    );

    // Restart confirmation
    const dialog = $('dm-restart-dialog');

    $('dm-restart').onclick = () => dialog.showModal();
    $('dm-restart-cancel').onclick = () => dialog.close();
    $('dm-restart-confirm').onclick = () => {
      dialog.close();
      save('/donglem/action', { action: 'restart' });
    };

    dialog.addEventListener('click', event => {
      if (event.target === dialog) dialog.close();
    });

    // Status updates
    document.addEventListener('visibilitychange', () => {
      if (!document.hidden) refresh();
    });

    refresh();
    setInterval(refresh, 2500);
  })();
</script>
)=====";

#endif  // _XDRV_124_DONGLE_M_WEB_H_
