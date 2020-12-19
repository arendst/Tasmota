#!/usr/bin/python3
#
# espupload by Theo Arends - 20170930
#
# Uploads binary file to OTA server
#
# Execute: espupload -u <Host_IP_address>:<Host_port>/<Host_path> -f <sketch.bin>
#
# Needs pycurl
#   - pip install pycurl

import sys
import os
import optparse
import logging
import pycurl

HOST_URL = "domus1:80/api/upload-arduino.php"

def upload(hostUrl, filename):
  tname = os.path.normpath(os.path.dirname(filename))
  new_filename = tname + os.sep + os.path.basename(tname) + '.bin'
  if os.path.exists(new_filename):
    os.remove(new_filename)
  os.rename(filename, new_filename)

  url = 'http://%s' % (hostUrl)
  c = pycurl.Curl()
  c.setopt(c.URL, url)
  # The "Expect:" is there to suppress "Expect: 100-continue" behaviour that is
  # the default in libcurl when posting large bodies (and fails on lighttpd).
  c.setopt(c.HTTPHEADER, ["Expect:"])
  c.setopt(c.HTTPPOST, [('file', (c.FORM_FILE, new_filename, )), ])
  c.perform()
  c.close()

def parser():
  parser = optparse.OptionParser(
    usage = "%prog [options]",
    description = "Upload image to over the air Host server for the esp8266 module with OTA support."
  )

  # destination ip and port
  group = optparse.OptionGroup(parser, "Destination")
  group.add_option("-u", "--host_url",
    dest = "host_url",
    action = "store",
    help = "Host url",
    default = HOST_URL
  )
  parser.add_option_group(group)

  # image
  group = optparse.OptionGroup(parser, "Image")
  group.add_option("-f", "--file",
    dest = "image",
    help = "Image file.",
    metavar = "FILE",
    default = None
  )
  parser.add_option_group(group)

  # output group
  group = optparse.OptionGroup(parser, "Output")
  group.add_option("-d", "--debug",
    dest = "debug",
    help = "Show debug output. And override loglevel with debug.",
    action = "store_true",
    default = False
  )
  parser.add_option_group(group)

  (options, args) = parser.parse_args()

  return options
# end parser

def main(args):
  # get options
  options = parser()

  # adapt log level
  loglevel = logging.WARNING
  if (options.debug):
    loglevel = logging.DEBUG
  # end if

  # logging
  logging.basicConfig(level = loglevel, format = '%(asctime)-8s [%(levelname)s]: %(message)s', datefmt = '%H:%M:%S')

  logging.debug("Options: %s", str(options))

  if (not options.host_url or not options.image):
    logging.critical("Not enough arguments.")

    return 1
  # end if

  if not os.path.exists(options.image):
    logging.critical('Sorry: the file %s does not exist', options.image)

    return 1
  # end if

  upload(options.host_url, options.image)
# end main

if __name__ == '__main__':
  sys.exit(main(sys.argv))
# end if