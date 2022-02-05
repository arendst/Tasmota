#!/usr/bin/env ruby

#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

### This script converts a bletiny log into a security manager unit test.  The
### input log must contain the connection establishment and complete pairing
### procedure.
###
### Arguments: None
### Stdin: bletiny log file

$PAIR_ALG_STRINGS = {
    0 => [ 'BLE_SM_PAIR_ALG_JW',        'just works',           'jw' ],
    1 => [ 'BLE_SM_PAIR_ALG_PASSKEY',   'passkey entry',        'pk' ],
    2 => [ 'BLE_SM_PAIR_ALG_OOB',       'out of band',          'ob' ],
    3 => [ 'BLE_SM_PAIR_ALG_NUMCMP',    'numeric comparison',   'nc' ]
}

$ADDR_TYPE_STRINGS = {
    0 => 'BLE_ADDR_TYPE_PUBLIC',
    1 => 'BLE_ADDR_TYPE_RANDOM',
    2 => 'BLE_ADDR_TYPE_RPA_PUB_DEFAULT',
    3 => 'BLE_ADDR_TYPE_RPA_RND_DEFAULT',
}

$ACTION_STRINGS = {
    0 => 'BLE_SM_IOACT_NONE',
    1 => 'BLE_SM_IOACT_OOB',
    2 => 'BLE_SM_IOACT_INPUT',
    3 => 'BLE_SM_IOACT_DISP',
    4 => 'BLE_SM_IOACT_NUMCMP',
}

$prev_idx = 0
$ctxt = {}

def test_case_name
    type_str = $ctxt[:sc] ? "sc" : "lgcy"
    init_str = $ctxt[:we_are_init] ? "us" : "peer"
    alg_str = $PAIR_ALG_STRINGS[$ctxt[:pair_alg]][2]
    iio_cap_str = "iio#{$ctxt[:pair_req][:io_cap]}"
    rio_cap_str = "rio#{$ctxt[:pair_rsp][:io_cap]}"
    bonding_str = "b#{$ctxt[:bonding] ? 1 : 0}"
    iat_str = "iat#{$ctxt[:addrs][:init_type]}"
    rat_str = "rat#{$ctxt[:addrs][:resp_type]}"
    ikey_str = "ik#{$ctxt[:pair_rsp][:init_key_dist]}"
    rkey_str = "rk#{$ctxt[:pair_rsp][:resp_key_dist]}"

    "ble_sm_" +
    "#{type_str}_#{init_str}_#{alg_str}_#{iio_cap_str}_#{rio_cap_str}_" +
    "#{bonding_str}_#{iat_str}_#{rat_str}_#{ikey_str}_#{rkey_str}"
end

def test_case_comment
<<-eos
/**
 * #{$ctxt[:sc] ? 'Secure connections' : 'Legacy'} pairing
 * Master: #{$ctxt[:we_are_init] ? "us" : "peer"}
 * Pair algorithm: #{$PAIR_ALG_STRINGS[$ctxt[:pair_alg]][1]}
 * Initiator IO capabilities: #{$ctxt[:pair_req][:io_cap]}
 * Responder IO capabilities: #{$ctxt[:pair_rsp][:io_cap]}
 * Bonding: #{$ctxt[:bonding]}
 * Initiator address type: #{$ADDR_TYPE_STRINGS[$ctxt[:addrs][:init_type]]}
 * Responder address type: #{$ADDR_TYPE_STRINGS[$ctxt[:addrs][:resp_type]]}
 * Initiator key distribution: #{$ctxt[:pair_rsp][:init_key_dist]}
 * Responder key distribution: #{$ctxt[:pair_rsp][:resp_key_dist]}
 */
eos
end

def to_hex_s(byte)
    if byte.is_a?(String)
        byte = s_to_i(byte)
    end

    "0x#{byte.to_s(16).rjust(2, '0')}"
end

# to_i(0) but interpret leading zeros as decimal.
def s_to_i(s)
    if s[0..1] == "0x"
        return s.to_i(16)
    else
        return s.to_i(10)
    end
end

def invalid_byte_line(msg, line)
    str = "invalid byte line"
    if msg != nil
        str += ": #{msg}"
    end

    str += "; line=#{line}"

    raise str
end

def token_string_to_bytes(line, delim = ' ')
    tokens = line.split(delim)
    bytes = []
    tokens.each do |token|
        begin
            byte = token.to_i(16)
            bytes << byte
        rescue
            invalid_byte_line("token=#{token}", line)
        end
    end

    return bytes
end

def txrx_prefix(is_tx)
    if is_tx
        return "tx"
    else
        return "rx"
    end
end

def reqrsp_s(is_req)
    reqrsp = nil
    if is_req
        return "req"
    else
        return "rsp"
    end
end

def bytes_to_arr_body(bytes, indent)
    lines = []

    idx = 0
    while idx < bytes.size
        slice_len = nil
        if bytes.size - idx >= 8
            slice_len = 8
        else
            slice_len = bytes.size - idx
        end

        slice = bytes[idx...(idx + slice_len)]
        line = ' ' * indent +
            slice.map{|b| to_hex_s(b)}.join(", ") + ","
        lines << line

        idx += slice_len
    end

    return lines.join("\n") << "\n"
end

def bytes_to_arr(bytes, name, indent)
    str = "#{' ' * indent}.#{name} = {\n"
    str << bytes_to_arr_body(bytes, indent + 4)
    str << "#{' ' * indent}},"

    return str
end

def addr_string_to_bytes(addr_string)
    token_string_to_bytes(addr_string, ':').reverse
end

def parse_pair_cmd(line, is_req)
    suffix = reqrsp_s(is_req)
    re = %r{
        pair\s#{suffix};
        \s
        conn=\d+
        \s
        io_cap=(?<io_cap>\d+)
        \s
        oob_data_flag=(?<oob_data_flag>\d+)
        \s
        authreq=(?<authreq>0x[0-9a-f]+)
        \s
        mac_enc_key_size=(?<max_enc_key_size>\d+)
        \s
        init_key_dist=(?<init_key_dist>\d+)
        \s
        resp_key_dist=(?<resp_key_dist>\d+)
    }x

    m = re.match(line)
    if m == nil
        return nil
    end

    cmd = {}
    cmd[:io_cap] = s_to_i(m[:io_cap])
    cmd[:oob_data_flag] = s_to_i(m[:oob_data_flag])
    cmd[:authreq] = s_to_i(m[:authreq])
    cmd[:max_enc_key_size] = s_to_i(m[:max_enc_key_size])
    cmd[:init_key_dist] = s_to_i(m[:init_key_dist])
    cmd[:resp_key_dist] = s_to_i(m[:resp_key_dist])

    return cmd
end

def parse_privkey(line)
    if !(line =~ /our privkey=(.+)/)
        return nil
    end
    return token_string_to_bytes($1)
end

def parse_public_key(line, is_tx)
    prefix = txrx_prefix(is_tx)
    if !(line =~ /#{prefix}ed sm command: public key; conn=\d+ x=(.+) y=(.+)/)
        return nil
    end

    pubkey = {}
    pubkey[:x] = token_string_to_bytes($1)
    pubkey[:y] = token_string_to_bytes($2)

    if pubkey[:x].size != 32
        raise "invalid public key: x length incorrect; line=#{line}"
    end

    if pubkey[:y].size != 32
        raise "invalid public key: y length incorrect; line=#{line}"
    end

    return pubkey
end

def parse_confirm(line, is_tx)
    prefix = txrx_prefix(is_tx)
    if !(line =~ /#{prefix}ed sm command: confirm; conn=\d+ value=(.+)/)
        return nil
    end

    bytes = token_string_to_bytes($1)
    if bytes.size != 16
        raise "invalid confirm line (length mismatch): #{line}"
    end

    return { :value => bytes }
end

def parse_random(line, is_tx)
    prefix = txrx_prefix(is_tx)
    if !(line =~ /#{prefix}ed sm command: random; conn=\d+ value=(.+)/)
        return nil
    end

    bytes = token_string_to_bytes($1)
    if bytes.size != 16
        raise "invalid random line (length mismatch): #{line}"
    end

    return { :value => bytes }
end

def parse_stk(line)
    if !(line =~ /^    out=(.+)/)
        return nil
    end

    bytes = token_string_to_bytes($1)
    if bytes.size != 16
        raise "invalid stk line (length mismatch): #{line}"
    end

    return bytes
end

def parse_dhkey_check(line, is_tx)
    prefix = txrx_prefix(is_tx)
    if !(line =~ /#{prefix}ed sm command: dhkey check; conn=\d+ value=(.+)/)
        return nil
    end

    bytes = token_string_to_bytes($1)
    if bytes.size != 16
        raise "invalid dhkey_check line (length mismatch): #{line}"
    end

    return { :value => bytes }
end

def parse_ltk(line)
    if !(line =~ /persisting.+ltk=([^ ]+)/)
        return nil
    end

    bytes = $1.split(":")
    if bytes.size != 16
        raise "invalid ltk line (length mismatch): exp=16 got=#{bytes.size} " +
              "line=#{line}"
    end

    return bytes
end

def parse_enc_info(line, is_tx)
    prefix = txrx_prefix(is_tx)
    if !(line =~ /#{prefix}ed sm command: enc info; conn=\d+ ltk=(.+)/)
        return nil
    end

    bytes = token_string_to_bytes($1)
    if bytes.size != 16
        raise "invalid enc info line (length mismatch): #{line}"
    end

    return { :ltk => bytes }
end

def parse_master_id(line, is_tx)
    prefix = txrx_prefix(is_tx)
    if !(line =~ /#{prefix}ed sm command: master id; conn=\d+ ediv=(.+) rand=(.+)/)
        return nil
    end

    return {
        :ediv => s_to_i($1),
        :rand => s_to_i($2),
    }
end

def parse_id_info(line, is_tx)
    prefix = txrx_prefix(is_tx)
    if !(line =~ /#{prefix}ed sm command: id info; conn=\d+ irk=(.+)/)
        return nil
    end

    bytes = token_string_to_bytes($1)
    if bytes.size != 16
        raise "invalid id info line (length mismatch): #{line}"
    end

    return { :irk => bytes }
end

def parse_id_addr_info(line, is_tx)
    prefix = txrx_prefix(is_tx)
    if !(line =~ /#{prefix}ed sm command: id addr info; conn=\d+ addr_type=(\d+) addr=(.+)/)
        return nil
    end

    bytes = addr_string_to_bytes($2)
    if bytes.size != 6
        raise "invalid id addr info line (length mismatch): #{line}"
    end

    return {
        :addr_type => s_to_i($1),
        :addr => bytes,
    }
end

def parse_sign_info(line, is_tx)
    prefix = txrx_prefix(is_tx)
    if !(line =~ /#{prefix}ed sm command: sign info; conn=\d+ sig_key=(.+)/)
        return nil
    end

    bytes = token_string_to_bytes($1)
    if bytes.size != 16
        raise "invalid sign info line (length mismatch): #{line}"
    end

    return {
        :sig_key => bytes,
    }
end

def parse_passkey_info(line)
    passkey_info = {}

    case line
    when /passkey action event; action=4 numcmp=(\d+)/
        passkey_info[:action] = 4
        passkey_info[:numcmp] = $1.to_i(10)
    when /^b passkey conn=\d+ action=1 oob=(\S+)/
        passkey_info[:action] = 1
        passkey_info[:oob] = token_string_to_bytes($1, ':')
    when /^b passkey conn=\d+ action=2 key=(\d+)/
        passkey_info[:action] = 2
        passkey_info[:key] = $1.to_i(10)
    when /b passkey conn=\d+ action=3 key=(\d+)/
        passkey_info[:action] = 3
        passkey_info[:key] = $1.to_i(10)
    else
        return nil
    end

    return passkey_info
end

def parse_addrs(line)
    if !(line =~ /our_ota_addr_type=(\d+) our_ota_addr=(\S+) our_id_addr_type=(\d+) our_id_addr=(\S+) peer_ota_addr_type=(\d+) peer_ota_addr=(\S+) peer_id_addr_type=(\d+) peer_id_addr=(\S+)/)
        return nil
    end

    our_ota_addr_bytes = addr_string_to_bytes($2)
    our_id_addr_bytes = addr_string_to_bytes($4)
    peer_ota_addr_bytes = addr_string_to_bytes($6)
    peer_id_addr_bytes = addr_string_to_bytes($8)

    if $ctxt[:we_are_init]
        init_id_bytes = our_id_addr_bytes
        init_ota_bytes = our_ota_addr_bytes
        resp_id_bytes = peer_id_addr_bytes
        resp_ota_bytes = peer_ota_addr_bytes
        init_addr_type = s_to_i($1)
        resp_addr_type = s_to_i($5)
    else
        init_id_bytes = peer_id_addr_bytes
        init_ota_bytes = peer_ota_addr_bytes
        resp_id_bytes = our_id_addr_bytes
        resp_ota_bytes = our_ota_addr_bytes
        init_addr_type = s_to_i($5)
        resp_addr_type = s_to_i($1)
    end

    if init_id_bytes == init_ota_bytes
        init_ota_bytes = [0] * 6
    end
    if resp_id_bytes == resp_ota_bytes
        resp_ota_bytes = [0] * 6
    end

    return {
        :init_type => init_addr_type,
        :resp_type => resp_addr_type,
        :init_id_addr => init_id_bytes,
        :resp_id_addr => resp_id_bytes,
        :init_rpa => init_ota_bytes,
        :resp_rpa => resp_ota_bytes,
    }
end

def detect_initiator(lines)
    lines.each do |line|
        if line =~ /txed sm command: pair req/
            $ctxt[:we_are_init] = true
        elsif line =~ /txed sm command: pair rsp/
            $ctxt[:we_are_init] = false
        end
    end

    if $ctxt[:we_are_init] == nil
        raise "could not detect which peer is the initiator"
    end
end

def pair_cmd_to_s(cmd, is_req)
    suffix = reqrsp_s(is_req)
    return <<-eos
        .pair_#{suffix} = {
            .io_cap = #{to_hex_s(cmd[:io_cap])},
            .oob_data_flag = #{to_hex_s(cmd[:oob_data_flag])},
            .authreq = #{to_hex_s(cmd[:authreq])},
            .max_enc_key_size = #{to_hex_s(cmd[:max_enc_key_size])},
            .init_key_dist = #{to_hex_s(cmd[:init_key_dist])},
            .resp_key_dist = #{to_hex_s(cmd[:resp_key_dist])},
        },
    eos
end

def privkey_to_s(privkey)
    return bytes_to_arr(privkey, "our_priv_key", 8)
end

def public_key_to_s(public_key, is_req)
    suffix = reqrsp_s(is_req)
    return <<-eos
        .public_key_#{suffix} = {
#{bytes_to_arr(public_key[:x], "x", 12)}
#{bytes_to_arr(public_key[:y], "y", 12)}
        },
    eos
end

def confirm_to_s(confirm, is_req, idx)
    return <<-eos
        .confirm_#{reqrsp_s(is_req)}[#{idx}] = {
#{bytes_to_arr(confirm[:value], "value", 12)}
        },
    eos
end

def random_to_s(random, is_req, idx)
    return <<-eos
        .random_#{reqrsp_s(is_req)}[#{idx}] = {
#{bytes_to_arr(random[:value], "value", 12)}
        },
    eos
end

def ltk_to_s(ltk)
    return bytes_to_arr(ltk, "ltk", 8)
end

def stk_to_s(stk)
    return bytes_to_arr(stk, "stk", 8)
end

def enc_info_to_s(id_info, is_req)
    return <<-eos
        .enc_info_#{reqrsp_s(is_req)} = {
#{bytes_to_arr(id_info[:ltk], "ltk", 12)}
        },
    eos
end

def master_id_to_s(master_id, is_req)
    return <<-eos
        .master_id_#{reqrsp_s(is_req)} = {
            .ediv = 0x#{master_id[:ediv].to_s(16)},
            .rand_val = 0x#{master_id[:rand].to_s(16)},
        },
    eos
end

def id_info_to_s(id_info, is_req)
    return <<-eos
        .id_info_#{reqrsp_s(is_req)} = {
#{bytes_to_arr(id_info[:irk], "irk", 12)}
        },
    eos
end

def id_addr_info_to_s(id_addr_info, is_req)
    return <<-eos
        .id_addr_info_#{reqrsp_s(is_req)} = {
            .addr_type = #{id_addr_info[:addr_type]},
#{bytes_to_arr(id_addr_info[:addr], "bd_addr", 12)}
        },
    eos
end

def sign_info_to_s(sign_info, is_req)
    return <<-eos
        .sign_info_#{reqrsp_s(is_req)} = {
#{bytes_to_arr(sign_info[:sig_key], "sig_key", 12)}
        },
    eos
end

def passkey_info_fill(passkey_info)
    case passkey_info[:action]
    # None
    when 0
        $ctxt[:pair_alg] = 0
        $ctxt[:authenticated] = false

    # OOB
    when 1
        $ctxt[:pair_alg] = 2
        $ctxt[:authenticated] = true

    # Input
    when 2
        $ctxt[:pair_alg] = 1
        $ctxt[:authenticated] = true

    # Display
    when 3
        $ctxt[:pair_alg] = 1
        $ctxt[:authenticated] = true

    # Numeric comparison
    when 4
        $ctxt[:pair_alg] = 3
        $ctxt[:authenticated] = true

    else
        raise "invalid MITM action: #{passkey_info[:action]}"
    end
end

def passkey_info_s
    passkey_info = $ctxt[:passkey_info]
    action_str = $ACTION_STRINGS[passkey_info[:action]]

    result = <<-eos
        .pair_alg = #{$ctxt[:pair_alg]},
        .authenticated = #{$ctxt[:authenticated]},
        .passkey_info = {
            .passkey = {
                .action = #{action_str},
    eos

    if passkey_info[:key] != nil
        result << <<-eos
                .passkey = #{passkey_info[:key].to_i},
        eos
    end
    if passkey_info[:oob] != nil
        result << <<-eos
#{bytes_to_arr(passkey_info[:oob], "oob", 16)}
        eos
    end
    if passkey_info[:numcmp] != nil
        result << <<-eos
                .numcmp_accept = 1,
        eos
    end

    result << <<-eos
            },
    eos

    if passkey_info[:numcmp] != nil
        result << <<-eos
            .exp_numcmp = #{passkey_info[:numcmp].to_i},
        eos
    end

    result << <<-eos
        },
    eos
end

def addrs_to_s(addrs)
    s = ''

    init_type = addrs[:init_type]
    resp_type = addrs[:resp_type]

    if init_type != 0
        s += "        .init_addr_type = #{$ADDR_TYPE_STRINGS[init_type]},\n"
    end
    s += bytes_to_arr(addrs[:init_id_addr], "init_id_addr", 8) + "\n"
    if init_type >= 2
        s += bytes_to_arr(addrs[:init_rpa], "init_rpa", 8) + "\n"
    end

    if resp_type != 0
        s += "        .resp_addr_type = #{$ADDR_TYPE_STRINGS[resp_type]},\n"
    end
    s += bytes_to_arr(addrs[:resp_id_addr], "resp_id_addr", 8) + "\n"
    if resp_type >= 2
        s += bytes_to_arr(addrs[:resp_rpa], "resp_rpa", 8) + "\n"
    end

    return s
end

def dhkey_check_to_s(dhkey_check, is_req)
    return <<-eos
        .dhkey_check_#{reqrsp_s(is_req)} = {
#{bytes_to_arr(dhkey_check[:value], "value", 12)}
        },
    eos
end

def extract_one(lines, ignore_prev = false)
    if ignore_prev
        start = 0
    else
        start = $prev_idx
    end

    (start...lines.size).each do |idx|
        line = lines[idx]
        result = yield(line)
        if result != nil
            if !ignore_prev
                $prev_idx = idx
            end
            return result
        end
    end

    return nil
end

def extract_pair_req(lines)
    return extract_one(lines) {|line| parse_pair_cmd(line, true)}
end

def extract_pair_rsp(lines)
    return extract_one(lines) {|line| parse_pair_cmd(line, false)}
end

def extract_privkey(lines)
    return extract_one(lines) {|line| parse_privkey(line)}
end

def extract_public_key_req(lines)
    return extract_one(lines) do |line|
        parse_public_key(line, $ctxt[:we_are_init])
    end
end

def extract_public_key_rsp(lines)
    return extract_one(lines) do |line|
        parse_public_key(line, !$ctxt[:we_are_init])
    end
end

def extract_confirm_req(lines)
    return extract_one(lines) do |line|
        parse_confirm(line, $ctxt[:we_are_init])
    end
end

def extract_confirm_rsp(lines)
    return extract_one(lines) do |line|
        parse_confirm(line, !$ctxt[:we_are_init])
    end
end

def extract_random_req(lines)
    return extract_one(lines) do |line|
        parse_random(line, $ctxt[:we_are_init])
    end
end

def extract_random_rsp(lines)
    return extract_one(lines) do |line|
        parse_random(line, !$ctxt[:we_are_init])
    end
end

def extract_confirm_random(lines)
    confirm_reqs = []
    confirm_rsps = []
    random_reqs = []
    random_rsps = []

    idx = 0
    loop do
        confirm_req = extract_confirm_req(lines)
        if confirm_req != nil
            confirm_reqs << confirm_req
        end

        confirm_rsp = extract_confirm_rsp(lines)
        break if confirm_rsp == nil
        if idx >= 20
            raise "too many confirm rsps (>20)"
        end
        confirm_rsps << confirm_rsp

        random_req = extract_random_req(lines)
        break if random_req == nil
        random_reqs << random_req

        random_rsp = extract_random_rsp(lines)
        break if random_rsp == nil
        random_rsps << random_rsp

        idx += 1
    end

    return confirm_reqs, confirm_rsps, random_reqs, random_rsps
end

def extract_stk(lines)
    return extract_one(lines, true) do |line|
        parse_stk(line)
    end
end

def extract_dhkey_check_req(lines)
    return extract_one(lines) do |line|
        parse_dhkey_check(line, $ctxt[:we_are_init])
    end
end

def extract_dhkey_check_rsp(lines)
    return extract_one(lines) do |line|
        parse_dhkey_check(line, !$ctxt[:we_are_init])
    end
end

def extract_enc_info_req(lines)
    return extract_one(lines) do |line|
        parse_enc_info(line, !$ctxt[:we_are_init])
    end
end

def extract_enc_info_rsp(lines)
    return extract_one(lines) do |line|
        parse_enc_info(line, $ctxt[:we_are_init])
    end
end

def extract_master_id_req(lines)
    return extract_one(lines) do |line|
        parse_master_id(line, !$ctxt[:we_are_init])
    end
end

def extract_master_id_rsp(lines)
    return extract_one(lines) do |line|
        parse_master_id(line, $ctxt[:we_are_init])
    end
end

def extract_id_info_req(lines)
    return extract_one(lines) do |line|
        parse_id_info(line, !$ctxt[:we_are_init])
    end
end

def extract_id_info_rsp(lines)
    return extract_one(lines) do |line|
        parse_id_info(line, $ctxt[:we_are_init])
    end
end

def extract_id_addr_info_req(lines)
    return extract_one(lines) do |line|
        parse_id_addr_info(line, !$ctxt[:we_are_init])
    end
end

def extract_id_addr_info_rsp(lines)
    return extract_one(lines) do |line|
        parse_id_addr_info(line, $ctxt[:we_are_init])
    end
end

def extract_sign_info_req(lines)
    return extract_one(lines) do |line|
        parse_sign_info(line, !$ctxt[:we_are_init])
    end
end

def extract_sign_info_rsp(lines)
    return extract_one(lines) do |line|
        parse_sign_info(line, $ctxt[:we_are_init])
    end
end

def extract_ltk(lines)
    return extract_one(lines) do |line|
        parse_ltk(line)
    end
end

def extract_passkey_info(lines)
    passkey_info = extract_one(lines, true) do |line|
        parse_passkey_info(line)
    end

    if passkey_info == nil
        passkey_info = { :action => 0 }
    end

    return passkey_info
end

def extract_addrs(lines)
    return extract_one(lines) do |line|
        parse_addrs(line)
    end
end


lines = STDIN.readlines

detect_initiator(lines)
$ctxt[:pair_req] = extract_pair_req(lines)
$ctxt[:pair_rsp] = extract_pair_rsp(lines)
$ctxt[:privkey] = extract_privkey(lines)
$ctxt[:public_key_req] = extract_public_key_req(lines)
$ctxt[:public_key_rsp] = extract_public_key_rsp(lines)
$ctxt[:confirm_reqs], $ctxt[:confirm_rsps], $ctxt[:random_reqs], $ctxt[:random_rsps] = extract_confirm_random(lines)
$ctxt[:passkey_info] = extract_passkey_info(lines)
$ctxt[:dhkey_check_req] = extract_dhkey_check_req(lines)
$ctxt[:dhkey_check_rsp] = extract_dhkey_check_rsp(lines)
$ctxt[:enc_info_req] = extract_enc_info_req(lines)
$ctxt[:master_id_req] = extract_master_id_req(lines)
$ctxt[:id_info_req] = extract_id_info_req(lines)
$ctxt[:id_addr_info_req] = extract_id_addr_info_req(lines)
$ctxt[:sign_info_req] = extract_sign_info_req(lines)
$ctxt[:enc_info_rsp] = extract_enc_info_rsp(lines)
$ctxt[:master_id_rsp] = extract_master_id_rsp(lines)
$ctxt[:id_info_rsp] = extract_id_info_rsp(lines)
$ctxt[:id_addr_info_rsp] = extract_id_addr_info_rsp(lines)
$ctxt[:sign_info_rsp] = extract_sign_info_rsp(lines)
$ctxt[:addrs] = extract_addrs(lines)
$ctxt[:ltk] = extract_ltk(lines)
$ctxt[:stk] = extract_stk(lines)

expected_confirm_rsps = nil
expected_random_reqs = nil
expected_random_rsps = nil
if $ctxt[:confirm_reqs].size == 0
    expected_confirm_rsps = 1
    expected_random_reqs = 1
    expected_random_rsps = 1
else
    expected_confirm_rsps = $ctxt[:confirm_reqs].size
    expected_random_reqs = $ctxt[:random_reqs].size
    expected_random_rsps = $ctxt[:random_rsps].size
end

if $ctxt[:confirm_rsps].size != expected_confirm_rsps
    raise "wrong number of confirm responses " +
          "(exp=#{expected_confirm_rsps}; got=#{$ctxt[:confirm_rsps].size}"
end

if $ctxt[:random_reqs].size != expected_random_reqs
    raise "wrong number of random requests " +
          "(exp=#{expected_random_reqs}; got=#{$ctxt[:random_reqs].size}"
end

if $ctxt[:random_rsps].size != expected_random_rsps
    raise "wrong number of random responses " +
          "(exp=#{expected_random_rsps}; got=#{$ctxt[:random_rsps].size}"
end

passkey_info_fill($ctxt[:passkey_info])

$ctxt[:sc] = $ctxt[:public_key_req] != nil
$ctxt[:bonding] = $ctxt[:pair_req][:authreq] & 1 == 1 &&
                  $ctxt[:pair_rsp][:authreq] & 1 == 1

puts test_case_comment()
puts <<-eos
TEST_CASE(#{test_case_name()})
{
    struct ble_sm_test_params params;

    params = (struct ble_sm_test_params) {
eos

puts addrs_to_s($ctxt[:addrs])

puts pair_cmd_to_s($ctxt[:pair_req], true)
puts pair_cmd_to_s($ctxt[:pair_rsp], false)

if $ctxt[:sc]
    puts privkey_to_s($ctxt[:privkey])
    puts public_key_to_s($ctxt[:public_key_req], true)
    puts public_key_to_s($ctxt[:public_key_req], false)
end

$ctxt[:confirm_rsps].size.times do |i|
    confirm_req = $ctxt[:confirm_reqs][i]
    confirm_rsp = $ctxt[:confirm_rsps][i]
    random_req = $ctxt[:random_reqs][i]
    random_rsp = $ctxt[:random_rsps][i]

    if confirm_req != nil
        puts confirm_to_s(confirm_req, true, i)
    end

    puts confirm_to_s(confirm_rsp, false, i)
    puts random_to_s(random_req, true, i)
    puts random_to_s(random_rsp, false, i)
end

if $ctxt[:sc]
    puts dhkey_check_to_s($ctxt[:dhkey_check_req], true)
    puts dhkey_check_to_s($ctxt[:dhkey_check_rsp], false)
end

if $ctxt[:enc_info_req] != nil
    puts enc_info_to_s($ctxt[:enc_info_req], true)
end
if $ctxt[:master_id_req] != nil
    puts master_id_to_s($ctxt[:master_id_req], true)
end
if $ctxt[:id_info_req] != nil
    puts id_info_to_s($ctxt[:id_info_req], true)
end
if $ctxt[:id_addr_info_req] != nil
    puts id_addr_info_to_s($ctxt[:id_addr_info_req], true)
end
if $ctxt[:sign_info_req] != nil
    puts sign_info_to_s($ctxt[:sign_info_req], true)
end
if $ctxt[:enc_info_rsp] != nil
    puts enc_info_to_s($ctxt[:enc_info_rsp], false)
end
if $ctxt[:master_id_rsp] != nil
    puts master_id_to_s($ctxt[:master_id_rsp], false)
end
if $ctxt[:id_info_rsp] != nil
    puts id_info_to_s($ctxt[:id_info_rsp], false)
end
if $ctxt[:id_addr_info_rsp] != nil
    puts id_addr_info_to_s($ctxt[:id_addr_info_rsp], false)
end
if $ctxt[:sign_info_rsp] != nil
    puts sign_info_to_s($ctxt[:sign_info_rsp], false)
end
if $ctxt[:sc]
    puts ltk_to_s($ctxt[:ltk])
else
    puts stk_to_s($ctxt[:stk])
end
puts passkey_info_s()

puts '    };'

if $ctxt[:sc]
    if $ctxt[:we_are_init]
        puts '    ble_sm_test_util_us_sc_good(&params);'
    else
        puts '    ble_sm_test_util_peer_sc_good(&params);'
    end
else
    if $ctxt[:we_are_init]
        puts '    ble_sm_test_util_us_lgcy_good(&params);'
    else
        puts '    ble_sm_test_util_peer_lgcy_good(&params);'
    end
end
puts '}'
