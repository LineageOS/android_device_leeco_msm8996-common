#!/usr/bin/env bash

adb root
adb shell mount -o rw,remount /vendor

adb push ./build/outputs/apk/debug/ConsumerirTransmitter.apk /vendor/priv-app/ConsumerirTransmitter

adb shell ps -ef | grep -iE 'org.lineageos.consumerirtransmitter' | grep -v grep | awk '{print $2}' | xargs adb shell kill -9

adb shell mount -o ro,remount /vendor
