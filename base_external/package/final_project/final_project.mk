
##############################################################
#
# Serial control user-space application
#
##############################################################

FINAL_PROJECT_VERSION = 'a54770f3fc38b93024482c37a6c38cdb70f74320'
# Note: Be sure to reference the *ssh* repository URL here (not https) to work properly
# with ssh keys and the automated build/test system.
# Your site should start with git@github.com:
FINAL_PROJECT_SITE = 'git@github.com:edautomation/embedded-linux-final-project.git'
FINAL_PROJECT_SITE_METHOD = git
FINAL_PROJECT_GIT_SUBMODULES = YES
FINAL_PROJECT_MODULE_SUBDIRS = serial_driver

define FINAL_PROJECT_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)/serial_control all
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)/serial_demo all
endef

define FINAL_PROJECT_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0644 $(@D)/serial_control/test_mapping.txt $(TARGET_DIR)/etc/test_mapping.txt
	$(INSTALL) -m 0755 $(@D)/serial_control/serial_control $(TARGET_DIR)/usr/bin/serial_control
	$(INSTALL) -m 0755 $(@D)/serial_demo/serial_demo $(TARGET_DIR)/usr/bin/serial_demo
	$(INSTALL) -m 0755 $(@D)/serial_driver/serial_modbus_load $(TARGET_DIR)/usr/bin/
	$(INSTALL) -m 0755 $(@D)/serial_driver/serial_modbus_unload $(TARGET_DIR)/usr/bin/
endef

$(eval $(kernel-module))
$(eval $(generic-package))
