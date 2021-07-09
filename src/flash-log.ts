import { css, html, HTMLTemplateResult, LitElement } from "lit";
import { customElement, state } from "lit/decorators.js";
import { classMap } from "lit/directives/class-map.js";
import { FlashState, State } from "./const";

interface Row {
  state?: State;
  message: HTMLTemplateResult | string;
  error?: boolean;
  action?: boolean;
}

@customElement("esp-web-flash-log")
export class FlashLog extends LitElement {
  @state() private _rows: Row[] = [];

  protected render() {
    return html`${this._rows.map(
      (row) =>
        html`<div
          class=${classMap({
            error: row.error === true,
            action: row.action === true,
          })}
        >
          ${row.message}
        </div>`
    )}`;
  }

  public willUpdate() {
    this.toggleAttribute("hidden", !this._rows.length);
  }

  public clear() {
    this._rows = [];
  }

  public processState(state: FlashState) {
    if (state.state === State.ERROR) {
      this.addError(state.message);
      return;
    }
    this.addRow(state);
    if (state.state === State.FINISHED) {
      this.addAction(
        html`<button @click=${this.clear}>Close this log</button>`
      );
    }
  }

  /**
   * Add or replace a row.
   */
  public addRow(row: Row) {
    // If last entry has same ID, replace it.
    if (
      row.state &&
      this._rows.length > 0 &&
      this._rows[this._rows.length - 1].state === row.state
    ) {
      const newRows = this._rows.slice(0, -1);
      newRows.push(row);
      this._rows = newRows;
    } else {
      this._rows = [...this._rows, row];
    }
  }

  /**
   * Add an error row
   */
  public addError(message: Row["message"]) {
    this.addRow({ message, error: true });
  }

  /**
   * Add an action row
   */
  public addAction(message: Row["message"]) {
    this.addRow({ message, action: true });
  }

  /**
   * Remove last row if state matches
   */
  public removeRow(state: string) {
    if (
      this._rows.length > 0 &&
      this._rows[this._rows.length - 1].state === state
    ) {
      this._rows = this._rows.slice(0, -1);
    }
  }

  static styles = css`
    :host {
      display: block;
      margin-top: 16px;
      padding: 12px 16px;
      font-family: monospace;
      background: var(--esp-tools-log-background, black);
      color: var(--esp-tools-log-text-color, greenyellow);
      font-size: 14px;
      line-height: 19px;
    }

    :host([hidden]) {
      display: none;
    }

    button {
      background: none;
      color: inherit;
      border: none;
      padding: 0;
      font: inherit;
      text-align: left;
      text-decoration: underline;
      cursor: pointer;
    }

    .error {
      color: var(--esp-tools-error-color, #dc3545);
    }

    .error,
    .action {
      margin-top: 1em;
    }
  `;
}

declare global {
  interface HTMLElementTagNameMap {
    "esp-web-flash-log": FlashLog;
  }
}
