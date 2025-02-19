#
# Create a style transition on a button to act like a gum when clicked
#

# Properties to transition
props = lv.style_prop_arr([lv.STYLE_TRANSFORM_WIDTH, lv.STYLE_TRANSFORM_HEIGHT, lv.STYLE_TEXT_LETTER_SPACE, 0])

# Transition descriptor when going back to the default state.
# Add some delay to be sure the press transition is visible even if the press was very short
transition_dsc_def = lv.style_transition_dsc(props, lv.anim_path_overshoot, 250, 100, nil)

# Transition descriptor when going to pressed state.
# No delay, go to pressed state immediately
transition_dsc_pr = lv.style_transition_dsc(props, lv.anim_path_ease_in_out, 250, 0, nil)

# Add only the new transition to the default state
style_def = lv.style()
style_def.set_transition(transition_dsc_def)

# Add the transition and some transformation to the presses state.
style_pr = lv.style()
style_pr.set_transform_width(10)
style_pr.set_transform_height(-10)
style_pr.set_text_letter_space(10)
style_pr.set_transition(transition_dsc_pr)

btn1 = lv.btn(lv.scr_act())
btn1.align(lv.ALIGN_CENTER, 0, -80)
btn1.add_style(style_pr, lv.STATE_PRESSED)
btn1.add_style(style_def, 0)

label = lv.label(btn1)
label.set_text("Gum")
