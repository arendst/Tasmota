# demo of gird layout

lv.start()

var col_dsc = lv.coord_arr([90, 90, 90, lv.GRID_TEMPLATE_LAST])
var row_dsc = lv.coord_arr([15, 15, 15, lv.GRID_TEMPLATE_LAST])
var scr = lv.scr_act()

var cont = lv.obj(scr)

cont.set_grid_dsc_array(col_dsc, row_dsc)
# replaces
# cont.set_style_grid_column_dsc_array(col_dsc, 0)
# cont.set_style_grid_row_dsc_array(row_dsc, 0)
# cont.set_layout(lv.LAYOUT_GRID)
cont.set_size(320, 240)
cont.center()

var zone_name = 0
var zone_mode = 1
var zone_power = 2
var zone_until = 3

var z1col = 0
var z2col = 1
var z3col = 2

# Zone Name

var lbl_z1_name = lv.label(cont)
var lbl_z2_name = lv.label(cont)
var lbl_z3_name = lv.label(cont)

lbl_z1_name.set_grid_cell(lv.GRID_ALIGN_CENTER, z1col, 1, lv.GRID_ALIGN_STRETCH, zone_name, 1)
lbl_z1_name.set_text("HTG 1")
lbl_z1_name.center()

lbl_z2_name.set_grid_cell(lv.GRID_ALIGN_CENTER, z2col, 1, lv.GRID_ALIGN_STRETCH, zone_name, 1)
lbl_z2_name.set_text("HTG 2")
lbl_z2_name.center()

lbl_z3_name.set_grid_cell(lv.GRID_ALIGN_CENTER, z3col, 1, lv.GRID_ALIGN_STRETCH, zone_name, 1)
lbl_z3_name.set_text("HTG 1")
lbl_z3_name.center()

# Zone Mode

var lbl_z1_mode = lv.label(cont)
var lbl_z2_mode = lv.label(cont)
var lbl_z3_mode = lv.label(cont)

lbl_z1_mode.set_grid_cell(lv.GRID_ALIGN_CENTER, z1col, 1, lv.GRID_ALIGN_STRETCH, zone_mode, 1)
lbl_z1_mode.set_text("Auto")
lbl_z1_mode.center()

lbl_z2_mode.set_grid_cell(lv.GRID_ALIGN_CENTER, z2col, 1, lv.GRID_ALIGN_STRETCH, zone_mode, 1)
lbl_z2_mode.set_text("Allday")
lbl_z2_mode.center()

lbl_z3_mode.set_grid_cell(lv.GRID_ALIGN_CENTER, z3col, 1, lv.GRID_ALIGN_STRETCH, zone_mode, 1)
lbl_z3_mode.set_text("Auto")
lbl_z3_mode.center()

# Zone Power

var lbl_z1_power = lv.label(cont)
var lbl_z2_power = lv.label(cont)
var lbl_z3_power = lv.label(cont)

lbl_z1_power.set_grid_cell(lv.GRID_ALIGN_CENTER, z1col, 1, lv.GRID_ALIGN_STRETCH, zone_power, 1)
lbl_z1_power.set_recolor(True)
lbl_z1_power.set_text("#ff0000 Off# Until")
lbl_z1_power.center()

lbl_z2_power.set_grid_cell(lv.GRID_ALIGN_CENTER, z2col, 1, lv.GRID_ALIGN_STRETCH, zone_power, 1)
lbl_z2_power.set_recolor(True)
lbl_z2_power.set_text("#0000ff On# Until")
lbl_z2_power.center()

lbl_z3_power.set_grid_cell(lv.GRID_ALIGN_CENTER, z3col, 1, lv.GRID_ALIGN_STRETCH, zone_power, 1)
lbl_z3_power.set_recolor(True)
lbl_z3_power.set_text("#ff0000 Off# Until")
lbl_z3_power.center()