#!/sbin/sh
#
# deunify
#

# Check if this is Lineage Recovery
LOSRECOVERY=/sbin/toybox

# DEVINFO
DEVINFO=$(strings /dev/block/sde21 | head -n 1)

echo "DEVINFO: ${DEVINFO}"

case "$DEVINFO" in
  le_zl0*)
    # Mount parittions
    if test -f "$LOSRECOVERY"; then
        toybox mount /dev/block/bootdevice/by-name/system -t ext4 /mnt/system
        toybox mount /dev/block/bootdevice/by-name/vendor -t ext4 /mnt/vendor
    else
        /tmp/toybox mount /dev/block/bootdevice/by-name/system -t ext4 /mnt/system
        /tmp/toybox mount /dev/block/bootdevice/by-name/vendor -t ext4 /mnt/vendor
    fi

    # Move firmware
    mv -f /mnt/vendor/firmware/zl0/* /mnt/vendor/firmware/

    # Remove NFC configs
    rm -f /mnt/system/system/etc/libnfc-nci.conf
    rm -f /mnt/vendor/etc/libnfc-nxp_RF.conf
    rm -f /mnt/vendor/etc/libnfc-nxp.conf

    # Remove NFC perms
    rm -f /mnt/vendor/etc/permissions/android.hardware.nfc*
    rm -f /mnt/vendor/etc/permissions/com.android.nfc*
    rm -f /mnt/vendor/etc/permissions/com.nxp.mifare.xml

    # Remove NFC services
    rm -f /mnt/vendor/etc/init/android.hardware.nfc*
    rm -f /mnt/vendor/etc/init/vendor.nxp.hardware.nfc*

    # Unmount partitions
    if test -f "$LOSRECOVERY"; then
        toybox umount /mnt/system
        toybox umount /mnt/vendor
    else
        /tmp/toybox umount /mnt/system
        /tmp/toybox umount /mnt/vendor
    fi
    ;;
  *)
    echo "Nothing to do!"
    ;;
esac

exit 0
