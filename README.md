# What is CFFT #
CFFT, Create File From Templates, just like its name
-- it lets you create files using certain existed templates.

CFFT works in terminal. It has a curses UI. 

# How to Use #

## Installation ##

### For Windows
I do not have machine in windows, so I don't know whether cfft can run in windows.

Maybe you can build it from source after modifing the source and installing the dependencies.

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

5. CFFT has **vim-like** key bindings, you can use **h/j/k/l** 
to control cursor move and choose which template you want to use.

### From AUR ###
You cannot install it from AUR because I haven't upload it right now :)

After everything is finished, I will upload it to AUR.

# To do #
- [x] Chinese and Nerd Font Support
- [x] Strage color
- [x] Better reminder(Such as warning or success notification)
- [x] File type judgement
- [x] Forbidden cfft run in templates dir
- [ ] Responsive Window
- [ ] Window size check
- [ ] User guide in UI
- [ ] configuration from file

# Bug Report #
If you don't want to use Github Issues,
you can contact me via Corona09@163.com

