#!/sbin/sh
#
# deunify
#

# DEVINFO
DEVINFO=$(strings /dev/block/sde21 | head -n 1)

echo "DEVINFO: ${DEVINFO}"

case "$DEVINFO" in
  le_zl0*)
    # Move firmware
    mv -f /mnt/vendor/firmware/zl0/* /mnt/vendor/firmware/

    # Remove NFC configs
    rm -f /mnt/system/etc/libnfc-nci.conf
    rm -f /mnt/vendor/etc/libnfc-nxp_RF.conf
    rm -f /mnt/vendor/etc/libnfc-nxp.conf

    # Remove NFC perms
    rm -f /mnt/vendor/etc/permissions/android.hardware.nfc*
    rm -f /mnt/vendor/etc/permissions/com.android.nfc*
    rm -f /mnt/vendor/etc/permissions/com.nxp.mifare.xml

    # Remove NFC services
    rm -f /mnt/vendor/etc/init/android.hardware.nfc*
    rm -f /mnt/vendor/etc/init/vendor.nxp.hardware.nfc*
    ;;
  *)
    echo "Nothing to do!"
    ;;
esac

exit 0
