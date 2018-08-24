#!/sbin/sh
#
# deunify
#

# DEVINFO
DEVINFO=$(cat /dev/block/sde21)

echo "DEVINFO: ${DEVINFO}"

case "$DEVINFO" in
  le_zl0*)
    # move firmware
    mv -f /system/vendor/firmware/zl0/* /system/vendor/firmware/

    # remove nfc perms
    rm -f /system/etc/permissions/android.hardware.nfc*
    rm -f /system/etc/permissions/com.android.nfc*

    # remove nfc service
    rm -f /system/vendor/etc/init/android.hardware.nfc*
    ;;
  *)
    echo "Nothing to do!"
    ;;
esac

exit 0
