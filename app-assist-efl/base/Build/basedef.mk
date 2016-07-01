# Add inputs and outputs from these tool invocations to the build variables

C_DEPS +=

SYSROOT := $(SBI_SYSROOT)

ROOTSTRAP_INCS := $(addprefix -I $(SYSROOT)/,$(PLATFORM_INCS_EX))
EFL_INCS := 

RS_LIBRARIES := $(addprefix -l,$(RS_LIBRARIES_EX))

PLATFORM_INCS := $(ROOTSTRAP_INCS) $(EFL_INCS) \
     -I"$(SDK_PATH)/library" 

