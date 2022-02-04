# What is CFFT #
CFFT, Create File From Templates, just like its name -- it lets you create files using certain existed templates.

CFFT works in terminal. It has a curses UI. 

# How to Use #

## Installation ##

### From Source Code ###
1. Download this repo at your wanted path
  ```
  git clone https://github.com/Corona09/cfft.git
  ```
2. run makefile
  ```Make
  sudo make install
  ```
3. Put your template file in `~/Templates`
  Of course, you can change it in `config.h`

4. In terminal, run `cfft`, and you can create file from template
in current working directory now.

### From AUR ###
You cannot install it from AUR because I haven't upload it right now :)

# Bug Report #
If you don't want to use Github Issues,
you can contact me via Corona09@163.com

---

1. `win_title`
    - cols = win_title.cols
    - rows = win_title.rows
    - start_col = PADDING_LEFT
    - start_row = PADDING_TOP
2. `win_list`
    - columns = win_title.cols
    - rows = LINES - PADDING_TOP - MARGIN_INTERVAL - PADDING_BOTTOM
    - start_col = PADDING_LEFT
    - start_row = PADDING_TOP + win_title.rows + MARGIN_INTERVAL
3. `win_preview`
    - start_col = PADDING_LEFT + win_title.cols + MARGIN_INTERVAL
    - start_row = PADDING_TOP
4. `win_input`
    - cols = COLS - PADDING_LEFT - PADDING_RIGHT
    - rows = 3
    - start_col = PADDING_LEFT
    - start_row = (LINES - rows) / 2
5. `win_msg`
    - cols = COLS - PADDING_LEFT - PADDING_RIGHT
    - rows = 3
    - start_col = PADDING_LEFT
    - start_row = (LINES - rows) / 2
