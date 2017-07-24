ifeq ($(OS),Windows_NT)
  CC=arduino.exe
else
  CC=arduino
endif

## Target executable ##
TGT_EXE_NAME=arduino_app
TGT_EXE=$(BUILD_PATH)/$(TGT_EXE_NAME).ino.bin
TGT_EXE_SRCS=$(TGT_EXE_NAME).ino
# TGT_EXE_DEPS=$(TGT_EXE_NAME).h
TGT_EXE_OBJS=$(TGT_EXE_NAME).hex
TGT_EXE_INCL=
BUILD_PATH=./output

$(TGT_EXE): $(TGT_EXE_SRCS) $(TGT_EXE_DEPS)
	$(CC) --verify arduino_app.ino --pref build.path=$(BUILD_PATH)

.PHONY=verify clean
verify: $(TGT_EXE)
clean:
	rm -rf $(BUILD_PATH)/*