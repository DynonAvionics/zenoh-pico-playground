#!/usr/bin/env bash

# NOTE: These patterns are based off what I see in the `pyocd list` output
# They might be incorrect for boards I'm not working with right now
stlink_id_pattern="[0-9A-F]{24}"
# Would use board_type_pattern only, but I think that could match part of the stlink_id
board_id_pattern="stm32[f-g][0-9]{3}[a-z]{2}tx"
board_type_pattern="[f-g][0-9]{3}[a-z]{2}"

readarray -t boards < <(pyocd list | grep -i stlink)

for board in "${boards[@]}"; do

    if [[ $board =~ $stlink_id_pattern ]]; then
        stlink_id=${BASH_REMATCH[0]}
        echo "stlink_id: $stlink_id"
    else
        echo "Problem: no stlink id found in: \"$board\""
        continue
    fi

    if [[ $board =~ $board_id_pattern ]]; then
        board_id=${BASH_REMATCH[0]}
        echo "board_id: $board_id"

        if [[ $board =~ $board_type_pattern ]]; then
            board_type=${BASH_REMATCH[0]}
            echo "board_type: $board_type"
        else
            echo "Problem: couldn't identify board type from: \"$board_id\""
            continue
        fi
    else
        echo "Problem: No board id found in: \"$board\""
        continue
    fi

    west_cmd="west flash -d build/nucleo_$board_type -r openocd --cmd-pre-init 'adapter serial $stlink_id'"
    echo -e "$west_cmd\n"

    eval "$west_cmd"
done