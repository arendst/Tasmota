#!/usr/bin/python
#
# espupload by Theo Arends - 20170103
#
# Uploads binary file to OTA server
#
# Execute: espupload -i <Host_IP_address> -p <Host_port> -f <sketch.bin>
#
# Needs pycurl
#   - pip install pycurl

import sys
import os
import optparse
import logging
import pycurl

HOST_ADDR = "domus1"
HOST_PORT = 80
HOST_URL = "/api/upload-arduino.php"

def upload(hostAddr, hostPort, hostUrl, filename):
  url = 'http://%s:%d%s' % (hostAddr, hostPort, hostUrl)
  c = pycurl.Curl()
  c.setopt(c.URL, url)
  # The "Expect:" is there to suppress "Expect: 100-continue" behaviour that is
  # the default in libcurl when posting large bodies (and fails on lighttpd).
  c.setopt(c.HTTPHEADER, ["Expect:"])
  c.setopt(c.HTTPPOST, [('file', (c.FORM_FILE, filename, )), ])
  c.perform()
  c.close()

def parser():
  parser = optparse.OptionParser(
    usage = "%prog [options]",
    description = "Upload image to over the air Host server for the esp8266 module with OTA support."
  )

  # destination ip, port and url
  group = optparse.OptionGroup(parser, "Destination")
  group.add_option("-i", "--host_ip",
    dest = "host_ip",
    action = "store",
    help = "Host IP Address. Default: " + HOST_ADDR,
    default = HOST_ADDR
  )
  group.add_option("-p", "--host_port",
    dest = "host_port",
    type = "int",
    help = "Host server ota Port. Default: " + str(HOST_PORT),
    default = HOST_PORT
  )
  group.add_option("-u", "--host_url",
    dest = "host_url",
    action = "store",
    help = "Host Url with / at beginning. Default: '" + HOST_URL + "'",
    default = HOST_URL
  )
  parser.add_option_group(group)

  # image
  group = optparse.OptionGroup(parser, "Image")
  group.add_option("-f", "--file",
    dest = "image",
    help = "Image file.",
    metavar="FILE",
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

  if (not options.host_ip or not options.image):
    logging.critical("Not enough arguments.")

    return 1
  # end if

  if not os.path.exists(options.image):
    logging.critical('Sorry: the file %s does not exist', options.image)

    return 1
  # end if

  upload(options.host_ip, options.host_port, options.host_url, options.image)
# end main

if __name__ == '__main__':
  sys.exit(main(sys.argv))
# end if
