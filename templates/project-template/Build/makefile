BUILD_SCRIPT_VERSION := 1.0.7

all : app_build

clean : app_clean

version : make_version


BSLASH := \\#
BSLASH2SLASH = $(subst $(BSLASH),/,$(1))
REMOVE_TAIL = $(patsubst %/,%,$(1))

PROJ_ROOT := $(call BSLASH2SLASH,$(PROJPATH))

-include $(PROJ_ROOT)/project_def.prop
-include basedef.mk
-include tooldef.mk
-include flags.mk
-include prepost.mk

APPTYPE := $(type)

OBJ_OUTPUT := $(call BSLASH2SLASH,$(OUTPUT_DIR)/objs)

OS_NAME = $(shell $(UNAME))

#LOWER_APPNAME := $(shell echo translit($(APPNAME),[A-Z],[a-z])|$(M4))
LOWER_APPNAME := $(shell echo $(APPNAME)|$(TR) [A-Z] [a-z])

ifeq ($(strip $(APPTYPE)),app)
APPFILE := $(OUTPUT_DIR)/$(LOWER_APPNAME)
endif
ifeq ($(strip $(APPTYPE)),staticLib)
APPFILE := $(OUTPUT_DIR)/lib$(LOWER_APPNAME).a
endif
ifeq ($(strip $(APPTYPE)),sharedLib)
APPFILE := $(OUTPUT_DIR)/lib$(LOWER_APPNAME).so
endif

ifneq ($(strip $(PLATFORM_INCS)),)
PLATFORM_INCS_FILE := $(OBJ_OUTPUT)/platform_incs_file.inc
endif

FIND_FILES = $(shell $(FIND) $(PROJ_ROOT)/$(dir $(1)) -type f -name $(notdir $(1)) -printf '$(dir $(1))%P ')

NORMAL_SRCS := $(filter-out %*.c %*.cpp,$(USER_SRCS))
WIDLCARD_SRCS := $(filter %*.c %*.cpp,$(USER_SRCS))
ALL_SRCS := $(NORMAL_SRCS) $(foreach var,$(WIDLCARD_SRCS),$(call FIND_FILES,$(var)))

C_SRCS := $(filter %.c,$(ALL_SRCS))
CPP_SRCS := $(filter %.cpp,$(ALL_SRCS))

USER_EDCS := $(subst $(BSLASH),/,$(USER_EDCS))
NORMAL_EDCS := $(filter-out %*.edc,$(USER_EDCS))
WIDLCARD_EDCS := $(filter %*.edc,$(USER_EDCS))
EDCS := $(NORMAL_EDCS) $(foreach var,$(WIDLCARD_EDCS),$(call FIND_FILES,$(var)))

NORMAL_POS := $(filter-out %*.po,$(USER_POS))
WIDLCARD_POS := $(filter %*.po,$(USER_POS))
POS := $(NORMAL_POS) $(foreach var,$(WIDLCARD_POS),$(call FIND_FILES,$(var)))

LIBPATHS := $(addprefix -L$(PROJ_ROOT)/,$(USER_LIB_DIRS)) \
			$(addprefix -L,$(USER_LIB_DIRS_ABS))
LIBS += $(addprefix -l,$(USER_LIBS))
UOBJS := $(addprefix $(PROJ_ROOT)/,$(USER_OBJS)) \
		 $(USER_OBJS_ABS)

M_OPT = -MMD -MP -MF"$(@:%.o=%.d)"

FUNC_C2O = $(patsubst %.c,$(OBJ_OUTPUT)/%.o,$(1))
FUNC_CPP2O = $(patsubst %.cpp,$(OBJ_OUTPUT)/%.o,$(1))
FUNC_EDC2EDJ = $(patsubst %.edc,$(OUTPUT_DIR)/%.edj,$(1))
FUNC_PO2MO = $(patsubst %.po,$(OUTPUT_DIR)/res/locale/%/LC_MESSAGES/$(LOWER_APPNAME).mo,$(notdir $(1)))


C_OBJS := $(call FUNC_C2O,$(C_SRCS)) 
CPP_OBJS := $(call FUNC_CPP2O,$(CPP_SRCS)) 
OBJS := $(C_OBJS) $(CPP_OBJS)
EDJ_FILES := $(call FUNC_EDC2EDJ,$(EDCS))
MO_FILES := $(call FUNC_PO2MO,$(POS))
DEPS := $(OBJS:.o=.d)


ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif

ifeq ($(strip $(APPTYPE)),app)
$(APPFILE) : $(OBJS) $(UOBJS)
	@echo '  Building target: $@'
	@echo '  Invoking: C/C++ Linker'
	@$(MKDIR) $(MKDIR_OP) $(subst $(BSLASH),/,$(@D))
	$(CXX) -o "$(APPFILE)" $(OBJS) $(UOBJS) $(LIBPATHS) -Xlinker --as-needed $(LIBS) $(LINK_FLAGS) $(TC_LINKER_MISC) $(RS_LINKER_MISC) -pie -lpthread -Xlinker -rpath="/home/developer/sdk_tools/lib" --sysroot="$(SYSROOT)" -Xlinker --version-script="$(PROJ_PATH)/.exportMap" -L"$(SBI_SYSROOT)/usr/lib" $(RS_LIBRARIES) -Xlinker -rpath="/opt/usr/apps/$(APPID)/lib" -Werror-implicit-function-declaration
	@echo '  Finished building target: $@'
endif
ifeq ($(strip $(APPTYPE)),staticLib)
$(APPFILE) : $(OBJS) $(UOBJS)
	@echo '  Building target: $@'
	@echo '  Invoking: Archive utility'
	@$(MKDIR) $(MKDIR_OP) $(subst $(BSLASH),/,$(@D))
	$(AR) -r "$(APPFILE)" $(OBJS) $(UOBJS) $(AR_FLAGS)
	@echo '  Finished building target: $@'
endif
ifeq ($(strip $(APPTYPE)),sharedLib)
$(APPFILE) : $(OBJS) $(UOBJS)
	@echo '  Building target: $@'
	@echo '  Invoking: C/C++ Linker'
	@$(MKDIR) $(MKDIR_OP) $(subst $(BSLASH),/,$(@D))
	$(CXX) -o "$(APPFILE)" $(OBJS) $(UOBJS) $(LIBPATHS) -Xlinker --as-needed $(LIBS) $(LINK_FLAGS) $(TC_LINKER_MISC) $(RS_LINKER_MISC) -shared -lpthread --sysroot="$(SYSROOT)" -L"$(SYSROOT)/usr/lib" $(RS_LIBRARIES)
	@echo '  Finished building target: $@'
endif


ifneq ($(strip $(C_SRCS)),)
CDEFS += $(addprefix -D,$(USER_DEFS))
CDEFS += $(addprefix -U,$(USER_UNDEFS))

INCS := $(addprefix -I$(PROJ_ROOT)/,$(USER_INC_DIRS)) \
		$(addprefix -I,$(USER_INC_DIRS_ABS)) \
		$(addprefix -include$(PROJ_ROOT)/,$(USER_INC_FILES)) \
		$(addprefix -include,$(USER_INC_FILES_ABS))

$(C_OBJS) : $(OBJ_OUTPUT)/%.o : $(PROJ_ROOT)/%.c $(PLATFORM_INCS_FILE)  
	@echo '  Building file: $<'
	@echo '  Invoking: C Compiler'
	@$(MKDIR) $(MKDIR_OP) $(subst $(BSLASH),/,$(@D))
ifeq ($(strip $(APPTYPE)),sharedLib)
	$(CC) -c $< -o $@ $(CDEFS) $(INCS) -I"pch" $(COMPILE_FLAGS) $(TC_COMPILER_MISC) $(RS_COMPILER_MISC) -fPIC --sysroot="$(SYSROOT)" -Werror-implicit-function-declaration $(M_OPT) @$(PLATFORM_INCS_FILE)
else
	$(CC) -c $< -o $@ $(CDEFS) $(INCS) -I"pch" $(COMPILE_FLAGS) $(TC_COMPILER_MISC) $(RS_COMPILER_MISC) -fPIE --sysroot="$(SYSROOT)" -Werror-implicit-function-declaration $(M_OPT) @$(PLATFORM_INCS_FILE)
endif
	@echo '  Finished building: $<'
endif

ifneq ($(strip $(CPP_SRCS)),)
CPPDEFS += $(addprefix -D,$(USER_CPP_DEFS))
CPPDEFS += $(addprefix -U,$(USER_CPP_UNDEFS))

CPP_INCS := $(addprefix -I$(PROJ_ROOT)/,$(USER_CPP_INC_DIRS)) \
		$(addprefix -I,$(USER_CPP_INC_DIRS_ABS)) \
		$(addprefix -include$(PROJ_ROOT)/,$(USER_CPP_INC_FILES)) \
		$(addprefix -include,$(USER_CPP_INC_FILES_ABS))
		
$(CPP_OBJS) : $(OBJ_OUTPUT)/%.o : $(PROJ_ROOT)/%.cpp $(PLATFORM_INCS_FILE)  
	@echo '  Building file: $<'
	@echo '  Invoking: C++ Compiler'
	@$(MKDIR) $(MKDIR_OP) $(subst $(BSLASH),/,$(@D))
ifeq ($(strip $(APPTYPE)),sharedLib)
	$(CXX) -c $< -o $@ $(CPPDEFS) $(CPP_INCS) -I"pch" $(CPP_COMPILE_FLAGS) $(TC_COMPILER_MISC) $(RS_COMPILER_MISC) -fPIC --sysroot="$(SYSROOT)" -Werror-implicit-function-declaration $(M_OPT) @$(PLATFORM_INCS_FILE)
else
	$(CXX) -c $< -o $@ $(CPPDEFS) $(CPP_INCS) -I"pch" $(CPP_COMPILE_FLAGS) $(TC_COMPILER_MISC) $(RS_COMPILER_MISC) -fPIE --sysroot="$(SYSROOT)" -Werror-implicit-function-declaration $(M_OPT) @$(PLATFORM_INCS_FILE)
endif
	@echo '  Finished building: $<'
endif


$(OBJ_OUTPUT) :
	@echo '  Building directory: $@'
	@$(MKDIR) $(MKDIR_OP) $(OBJ_OUTPUT)

preproc : $(OBJ_OUTPUT)
ifneq ($(strip $(PREBUILD_COMMAND)),)
ifneq ($(strip $(PREBUILD_DESC)),)
	@echo $(PREBUILD_DESC)
endif	
	@echo $(shell $(PREBUILD_COMMAND))
endif	


postproc :
ifneq ($(strip $(POSTBUILD_COMMAND)),)
ifneq ($(strip $(POSTBUILD_DESC)),)
	@echo $(POSTBUILD_DESC)
endif	
	@echo $(shell $(POSTBUILD_COMMAND))
endif	

ifneq ($(strip $(PLATFORM_INCS)),)
$(PLATFORM_INCS_FILE) : $(OBJ_OUTPUT)
	@echo '  Building inc file: $@'
ifneq ($(findstring Linux,$(OS_NAME)),)
	@echo $(PLATFORM_INCS) > $@
else
ifneq ($(findstring 3.82,$(MAKE_VERSION)),)
	$(file > $@,$(PLATFORM_INCS))
else
	@echo $(PLATFORM_INCS) > $@
endif	 
endif
endif

ALL_EDJ_FILES :=

define EDJ_PROC
ALL_EDCS := $$(subst $(BSLASH),/,$$($(2)))
NORMAL_EDCS := $$(filter-out %*.edc,$$(ALL_EDCS))
WIDLCARD_EDCS := $$(filter %*.edc,$$(ALL_EDCS))
EDCS := $$(NORMAL_EDCS) $$(foreach var_edc,$$(WIDLCARD_EDCS),$$(call FIND_FILES,$$(var_edc)))

ifneq ($$(strip $$(EDCS)),)
EDJ_FILES := $$(call FUNC_EDC2EDJ,$$(EDCS))

ALL_EDJ_FILES += $$(EDJ_FILES)

EDC_$(1)_COMPILER_FLAGS := -id "$$(SDK_TOOLPATH)/enventor/share/enventor/images"
EDC_$(1)_COMPILER_FLAGS += -sd "$$(SDK_TOOLPATH)/enventor/share/enventor/sounds"
EDC_$(1)_COMPILER_FLAGS += -fd "$$(SDK_TOOLPATH)/enventor/share/enventor/fonts"

ifneq ($$(strip $(3)),)
EDC_$(1)_COMPILER_FLAGS += $$(addprefix -id $$(PROJ_PATH)/,$$($(3)))
endif
ifneq ($$(strip $(4)),)
EDC_$(1)_COMPILER_FLAGS += $$(addprefix -id ,$$($(4)))
endif
ifneq ($$(strip $(5)),)
EDC_$(1)_COMPILER_FLAGS += $$(addprefix -sd $$(PROJ_PATH)/,$$($(5)))
endif
ifneq ($$(strip $(6)),)
EDC_$(1)_COMPILER_FLAGS += $$(addprefix -sd ,$$($(6)))
endif
ifneq ($$(strip $(7)),)
EDC_$(1)_COMPILER_FLAGS += $$(addprefix -fd $$(PROJ_PATH)/,$$($(7)))
endif
ifneq ($$(strip $(8)),)
EDC_$(1)_COMPILER_FLAGS += $$(addprefix -fd ,$$($(8)))
endif

$$(EDJ_FILES) : $$(OUTPUT_DIR)/%.edj : $$(PROJ_ROOT)/%.edc 
	@echo '  Building file: $$<'
	@echo '  Invoking: EDC Resource Compiler'
	@$$(MKDIR) $$(MKDIR_OP) $$(subst $$(BSLASH),/,$$(@D))
	$$(EDJE_CC) $$(EDC_$(1)_COMPILER_FLAGS) $$(CDEFS) "$$<" "$$@"
	@echo '  Finished building: $$<'
endif
endef

# Global EDCs
ifneq ($(strip $(USER_EDCS)),)
$(eval $(call EDJ_PROC,,USER_EDCS,USER_EDCS_IMAGE_DIRS,USER_EDCS_IMAGE_DIRS_ABS,USER_EDCS_SOUND_DIRS,USER_EDCS_SOUND_DIRS_ABS,USER_EDCS_FONT_DIRS,USER_EDCS_FONT_DIRS_ABS))
endif

# Individual EDCs
ifneq ($(strip $(USER_EXT_EDC_KEYS)),)
$(foreach var,$(USER_EXT_EDC_KEYS),$(eval $(call EDJ_PROC,$(var),USER_EXT_$(var)_EDCS,USER_EXT_$(var)_EDCS_IMAGE_DIRS,USER_EXT_$(var)_EDCS_IMAGE_DIRS_ABS,USER_EXT_$(var)_EDCS_SOUND_DIRS,USER_EXT_$(var)_EDCS_SOUND_DIRS_ABS,USER_EXT_$(var)_EDCS_FONT_DIRS,USER_EXT_$(var)_EDCS_FONT_DIRS_ABS)))
endif


ifneq ($(strip $(POS)),)
define MO_RULE
$(call FUNC_PO2MO,$(1)) : $(PROJ_ROOT)/$(1)
	@echo '  Building file: $$<'
	@echo '  Invoking: msgfmt String Formatter'
	@$$(MKDIR) $$(MKDIR_OP) $$(subst $$(BSLASH),/,$$(@D))
	$$(MSGFMT) -o $$@ $$<
	@echo '  Finished building: $$<'
endef

$(foreach var,$(POS),$(eval $(call MO_RULE,$(var))))
endif


secondary-outputs : $(ALL_EDJ_FILES) $(MO_FILES)

-include appendix.mk


app_build : preproc $(APPFILE) secondary-outputs postproc

app_clean :
	rm -f "$(APPFILE)"
	rm -rf "$(OUTPUT_DIR)"

make_version :
	@echo $(BUILD_SCRIPT_VERSION)
