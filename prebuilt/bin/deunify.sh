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
    mv -f /vendor/firmware/zl0/* /vendor/firmware/

    # remove nfc perms
    rm -f /vendor/etc/permissions/android.hardware.nfc*
    rm -f /vendor/etc/permissions/com.android.nfc*

    # remove nfc service
    rm -f /vendor/etc/init/android.hardware.nfc*
    ;;
  *)
    echo "Nothing to do!"
    ;;
esac

exit 0
