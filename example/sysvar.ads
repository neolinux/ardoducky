press gui+r
wait 200
write notepad
press enter
wait 200

ifj $caps capslock
write capslock not enabled!
goto end

:capslock
write capslock enabled!

:end
