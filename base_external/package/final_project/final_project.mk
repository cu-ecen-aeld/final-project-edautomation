
##############################################################
#
# Serial control user-space application
#
##############################################################

FINAL_PROJECT_VERSION = '947202347c777023cf4fcc2dd78ebe10e7063b55'
# Note: Be sure to reference the *ssh* repository URL here (not https) to work properly
# with ssh keys and the automated build/test system.
# Your site should start with git@github.com:
FINAL_PROJECT_SITE = 'git@github.com:edautomation/embedded-linux-final-project.git'
FINAL_PROJECT_SITE_METHOD = git
FINAL_PROJECT_GIT_SUBMODULES = YES
# FINAL_PROJECT_MODULE_SUBDIRS = serial_control

define FINAL_PROJECT_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)/serial_control all
endef

define FINAL_PROJECT_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/serial_control/serial_control $(TARGET_DIR)/usr/bin/serial_control
endef

# $(eval $(kernel-module))
$(eval $(generic-package))
