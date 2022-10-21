# Create a chart
chart = lv.chart(lv.scr_act())
chart.set_size(200, 150)
chart.center()
chart.set_type(lv.CHART_TYPE_LINE)   # Show lines and points too

# Add two data series
ser1 = chart.add_series(lv.palette_main(lv.PALETTE_RED), lv.CHART_AXIS_PRIMARY_Y)
ser2 = chart.add_series(lv.palette_main(lv.PALETTE_GREEN), lv.CHART_AXIS_SECONDARY_Y)

# Set next points on ser1
chart.set_next_value(ser1,10)
chart.set_next_value(ser1,10)
chart.set_next_value(ser1,10)
chart.set_next_value(ser1,10)
chart.set_next_value(ser1,10)
chart.set_next_value(ser1,10)
chart.set_next_value(ser1,10)
chart.set_next_value(ser1,30)
chart.set_next_value(ser1,70)
chart.set_next_value(ser1,90)

# Directly set points on 'ser2'
y2 = lv.coord_arr(ser2.y_points, 10)
y2[0] = 90
y2[1] = 70
y2[2] = 65
y2[3] = 65
y2[4] = 65
y2[5] = 65
y2[6] = 65
y2[7] = 65
y2[8] = 65
y2[9] = 65
chart.refresh()      #  Required after direct set

