#! /usr/bin/env python3
import subprocess
import json
import argparse
import sys
import logging


def main():
    parser = argparse.ArgumentParser()
    parser.description = u'Compile test a sketch for all available boards'
    parser.add_argument(u'-s', u'--sketch', dest=u'sketch',
                        required=True, help=u'Path to sketch')
    args = parser.parse_args()
    test_all_boards(args.sketch)


def test_all_boards(sketch):
    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s [%(levelname)s] %(message)s')
    log = logging.getLogger('arduino-compile-test')
    process = subprocess.run("arduino-cli board listall --format json".split(),
                             stdout=subprocess.PIPE)
    board_list_json = process.stdout.decode('utf-8')
    board_list = json.loads(board_list_json)
    test_list = ["arduino:samd:mkrzero", "arduino:avr:mega",
                 "arduino:avr:nano", "arduino:avr:uno",
                 "esp32:esp32:esp32", "esp8266:esp8266:generic"]
    for board in test_list:
        if board in (b['fqbn'] for b in board_list['boards']):
            log.info('Test compilation for board {}'.format(board))
            command = 'arduino-cli compile  --libraries="." --warnings all'\
                      ' --fqbn {board} {sketch}'.format(board=board,
                                                        sketch=sketch)
            process = subprocess.run(command.split(), stdout=subprocess.PIPE)
            if process.returncode:
                log.error(process.stdout.decode('utf-8'))
                sys.exit(process.returncode)
        else:
            log.error('Board not installed: {}'.format(board))
            sys.exit(-1)


if __name__ == '__main__':
    main()
