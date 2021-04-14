[CCode (cprefix = "mpd_", cheader_filename = "mpd/client.h")]

namespace Mpd {
	[CCode (cname = "enum mpd_error")]
	public enum Error {
		SUCCESS = 0,
		OOM,
		ARGUMENT,
		STATE,
		TIMEOUT,
		SYSTEM,
		RESOLVER,
		MALFORMED,
		CLOSED,
		SERVER
	}

	[CCode (cname = "enum mpd_async_event")]
	public enum AsyncEvent {
		READ = 1,
		WRITE = 2,
		HUP = 4,
		ERROR = 8
	}

	[CCode (cname = "enum mpd_server_error")]
	public enum ServerError {
		UNK = -1,
		NOT_LIST = 1,
		ARG = 2,
		PASSWORD = 3,
		PERMISSION = 4,
		UNKNOWN_CMD = 5,
		NO_EXIST = 50,
		PLAYLIST_MAX = 51,
		SYSTEM = 52,
		PLAYLIST_LOAD = 53,
		UPDATE_ALREADY = 54,
		PLAYER_SYNC = 55,
		ERROR_EXIST = 56
	}

	[CCode (cname = "enum mpd_entity_type")]
	public enum EntityType {
		UNKNOWN,
		DIRECTORY,
		SONG,
		PLAYLIST
	}

	[CCode (cname = "enum mpd_idle")]
	public enum Idle {
		DATABASE = 0x1,
		STORED_PLAYLIST = 0x2,
		QUEUE = 0x4,
		PLAYLIST = QUEUE,
		PLAYER = 0x8,
		MIXER = 0x10,
		OUTPUT = 0x20,
		OPTIONS = 0x40,
		UPDATE = 0x80,
		STICKER = 0x100,
		SUBSCRIPTION = 0x200,
		MESSAGE = 0x400
	}

	[CCode (cname = "enum mpd_parser_result")]
	public enum ParserResult {
		MALFORMED,
		SUCCESS,
		ERROR,
		PAIR
	}

	[CCode (cname = "enum mpd_operator")]
	public enum Operator {
		DEFAULT
	}

	[CCode (cname = "enum mpd_tag_type",
		cprefix = "MPD_TAG_")]
	public enum TagType {
		UNKNOWN = -1,
		ARTIST,
		ALBUM,
		ALBUM_ARTIST,
		TITLE,
		TRACK,
		NAME,
		GENRE,
		DATE,
		COMPOSER,
		PERFORMER,
		COMMENT,
		DISC,
		MUSICBRAINZ_ARTISTID,
		MUSICBRAINZ_ALBUMID,
		MUSICBRAINZ_ALBUMARTISTID,
		MUSICBRAINZ_TRACKID,
		MUSICBRAINZ_RELEASETRACKID,
		COUNT
	}

	[CCode (cname = "enum mpd_state")]
	public enum State {
		UNKNOWN = 0,
		STOP = 1,
		PLAY = 2,
		PAUSE = 3
	}

	[CCode (cname = "struct mpd_async",
		free_function = "mpd_async_free")]
	[Compact]
	public class Async {
		public Mpd.Error error { get; }
		public string? error_message { get; }
		public int system_error { get; }
		public int fd { get; }

		public Async(int fd);
		public Mpd.Error get_error();
		public unowned string? get_error_message();
		public int get_system_error();
		public int get_fd();
		public Mpd.AsyncEvent events();
		public bool async_io(Mpd.AsyncEvent events);
		public bool send_command_v(string command, va_list args);
		public bool send_command(string command, ...);
		public string? recv_line();
	}

	[CCode (cname = "struct mpd_audio_format")]
	[Compact]
	public class AudioFormat {
		public uint8 bits;
		public uint8 channels;
		public uint32 sample_rate;
	}

	[CCode (cname = "struct mpd_connection",
		free_function="mpd_connection_free")]
	[Compact]
	public class Connection {
		public int fd { get; }
		public Mpd.Async async { get; }
		public Mpd.Error error { get; }
		public string error_message { get; }
		public Mpd.ServerError server_error { get; }
		public int system_error { get; }
		public uint[] server_version { get; }
		public uint server_error_location { get; }
		public uint timeout { set; }
		public Settings settings { get; }

		public Connection(string? host = null, uint port = 0, uint timeout_ms = 0);
		[CCode (cname = "mpd_connection_new_async")]
		public Connection.from_async(Mpd.Async async, string welcome);
		public void set_timeout(uint timeout_ms);
		public int get_fd();
		public Mpd.Async get_async();
		public Mpd.Error get_error();
		public unowned string get_error_message();
		public Mpd.ServerError get_server_error();
		public uint get_server_error_location();
		public int get_system_error();
		public bool clear_error();
		public uint[] get_server_version();
		public int cmp_server_version(uint major, uint minor, uint patch);
		public Settings get_settings();
		[CCode (cname = "mpd_recv_directory")]
		public Directory? recv_directory();
		[CCode (cname = "mpd_recv_entity")]
		public Entity? recv_entity();
		[CCode (cname = "mpd_command_list_begin")]
		public bool command_list_begin(bool discrete_ok = false);
		[CCode (cname = "mpd_command_list_end")]
		public bool command_list_end();
		[CCode (cname = "mpd_send_set_volume")]
		public bool send_set_volume(uint volume);
		[CCode (cname = "mpd_run_set_volume")]
		public bool run_set_volume(uint volume);
		[CCode (cname = "mpd_send_outputs")]
		public bool send_outputs();
		[CCode (cname = "mpd_recv_output")]
		public Output recv_output();
		[CCode (cname = "mpd_send_enable_output")]
		public bool send_enable_output(uint output_id);
		[CCode (cname = "mpd_run_enable_output")]
		public bool run_enable_output(uint output_id);
		[CCode (cname = "mpd_send_disable_output")]
		public bool send_disable_output(uint output_id);
		[CCode (cname = "mpd_run_disable_output")]
		public bool run_disable_output(uint output_id);
		[CCode (cname = "mpd_send_password")]
		public bool send_password(string password);
		[CCode (cname = "mpd_run_password")]
		public bool run_password(string password);
		[CCode (cname = "mpd_recv_playlist")]
		public Playlist recv_playlist();
		[CCode (cname = "mpd_send_list_playlist")]
		public bool send_list_playlist(string name);
		[CCode (cname = "mpd_send_list_playlist_meta")]
		public bool send_list_playlist_meta(string name);
		[CCode (cname = "mpd_send_playlist_clear")]
		public bool send_playlist_clear(string name);
		[CCode (cname = "mpd_run_playlist_clear")]
		public bool run_playlist_clear(string name);
		[CCode (cname = "mpd_send_playlist_add")]
		public bool send_playlist_add(string name, string path);
		[CCode (cname = "mpd_run_playlist_add")]
		public bool run_playlist_add(string name, string path);
		[CCode (cname = "mpd_send_playlist_move")]
		public bool send_playlist_move(string name, uint from, uint to);
		[CCode (cname = "mpd_send_playlist_delete")]
		public bool send_playlist_delete(string name, uint pos);
		[CCode (cname = "mpd_run_playlist_delete")]
		public bool run_playlist_delete(string name, uint pos);
		[CCode (cname = "mpd_send_save")]
		public bool send_save(string name);
		[CCode (cname = "mpd_run_save")]
		public bool run_save(string name);
		[CCode (cname = "mpd_send_load")]
		public bool send_load(string name);
		[CCode (cname = "mpd_run_load")]
		public bool run_load(string name);
		[CCode (cname = "mpd_send_rename")]
		public bool send_rename(string from, string to);
		[CCode (cname = "mpd_run_rename")]
		public bool run_rename(string from, string to);
		[CCode (cname = "mpd_send_rm")]
		public bool send_rm(string name);
		[CCode (cname = "mpd_run_rm")]
		public bool run_rm(string name);
		[CCode (cname = "mpd_recv_pair")]
		public Pair recv_pair();
		[CCode (cname = "mpd_recv_pair_named")]
		public Pair recv_pair_named(string name);
		[CCode (cname = "mpd_return_pair")]
		public void return_pair(Pair pair);
		[CCode (cname = "mpd_enqueue_pair")]
		public void enqueue_pair(Pair pair);
		[CCode (cname = "mpd_response_finish")]
		public bool response_finish();
		[CCode (cname = "mpd_response_next")]
		public bool response_next();
		[CCode (cname = "mpd_search_db_songs")]
		public bool search_db_songs(bool exact);
		[CCode (cname = "mpd_search_add_db_songs")]
		public bool search_add_db_songs(bool exact);
		[CCode (cname = "mpd_search_queue_songs")]
		public bool search_queue_songs(bool exact);
		[CCode (cname = "mpd_search_db_tags")]
		public bool search_db_tags(TagType type);
		[CCode (cname = "mpd_count_db_songs")]
		public bool count_db_songs();
		[CCode (cname = "mpd_search_add_uri_constraint")]
		public bool search_add_uri_constraint(Operator oper, string value);
		[CCode (cname = "mpd_search_add_tag_constraint")]
		public bool search_add_tag_constraint(Operator oper, TagType type, string value);
		[CCode (cname = "mpd_search_add_any_tag_constraint")]
		public bool search_add_any_tag_constraint(Operator oper, string value);
		[CCode (cname = "mpd_search_commit")]
		public bool search_commit();
		[CCode (cname = "mpd_search_cancel")]
		public bool search_cancel();
		[CCode (cname = "mpd_recv_pair_tag")]
		public Pair recv_pair_tag(TagType type);
		[CCode (cname = "mpd_send_command")]
		public bool send_command(string command, ...);
		[CCode (cname = "mpd_recv_song")]
		public Song recv_song();
		[CCode (cname = "mpd_send_status")]
		public bool send_status();
		[CCode (cname = "mpd_recv_status")]
		public Status recv_status();
		[CCode (cname = "mpd_run_status")]
		public Status run_status();
		[CCode (cname = "mpd_send_allowed_commands")]
		public bool send_allowed_commands();
		[CCode (cname = "mpd_send_disallowed_commands")]
		public bool send_disallowed_commands();
		[CCode (cname = "mpd_recv_command_pair")]
		public Pair? recv_command_pair();
		[CCode (cname = "mpd_send_list_url_schemes")]
		public bool send_list_url_schemas();
		[CCode (cname = "mpd_recv_url_scheme_pair")]
		public Pair? recv_url_scheme_pair();
		[CCode (cname = "mpd_send_list_tag_types")]
		public bool send_list_tag_types();
		[CCode (cname = "mpd_recv_tag_type_pair")]
		public Pair? recv_tag_type_pair();
		[CCode (cname = "mpd_send_list_all")]
		public bool send_list_all(string path);
		[CCode (cname = "mpd_send_list_all_meta")]
		public bool send_list_all_meta(string path);
		[CCode (cname = "mpd_send_list_meta")]
		public bool send_list_meta(string path);
		[CCode (cname = "mpd_send_update")]
		public bool send_update(string? path = null);
		[CCode (cname = "mpd_send_rescan")]
		public bool send_rescan(string? path = null);
		[CCode (cname = "mpd_recv_update_id")]
		public uint recv_update_id();
		[CCode (cname = "mpd_run_update")]
		public uint run_update(string? path = null);
		[CCode (cname = "mpd_run_rescan")]
		public uint run_rescan(string? path = null);
		[CCode (cname = "mpd_send_idle")]
		public bool send_idle();
		[CCode (cname = "mpd_send_idle_mask")]
		public bool send_idle_mask(Idle mask);
		[CCode (cname = "mpd_send_noidle")]
		public bool send_noidle();
		[CCode (cname = "mpd_recv_idle")]
		public Idle recv_idle(bool disable_timeout);
		[CCode (cname = "mpd_run_idle")]
		public Idle run_idle();
		[CCode (cname = "mpd_run_idle_mask")]
		public Idle run_idle_mask(Idle mask);
		[CCode (cname = "mpd_run_noidle")]
		public Idle run_noidle();
		[CCode (cname = "mpd_send_stats")]
		public bool send_stats();
		[CCode (cname = "mpd_recv_stats")]
		public Stats recv_stats();
		[CCode (cname = "mpd_run_stats")]
		public Stats run_stats();
		[CCode (cname = "mpd_send_sticker_set")]
		public bool send_sticker_set(string type, string uri, string name, string value);
		[CCode (cname = "mpd_run_sticker_set")]
		public bool run_sticker_set(string type, string uri, string name, string value);
		[CCode (cname = "mpd_send_sticker_delete")]
		public bool send_sticker_delete(string type, string uri, string name);
		[CCode (cname = "mpd_run_sticker_delete")]
		public bool run_sticker_delete(string type, string uri, string name);
		[CCode (cname = "mpd_send_sticker_get")]
		public bool send_sticker_get(string type, string uri, string name);
		[CCode (cname = "mpd_send_sticker_list")]
		public bool send_sticker_list(string type, string uri);
		[CCode (cname = "mpd_send_sticker_find")]
		public bool send_sticker_find(string type, string base_uri, string name);
		[CCode (cname = "mpd_recv_sticker")]
		public Pair recv_sticker();
		[CCode (cname = "mpd_return_sticker")]
		public void return_sticker(Pair pair);
		[CCode (cname = "mpd_send_list_queue_meta")]
		public bool send_list_queue_meta();
		[CCode (cname = "mpd_send_list_queue_range_meta")]
		public bool send_list_queue_range_meta(uint start, uint end);
		[CCode (cname = "mpd_send_get_queue_song_pos")]
		public bool send_get_queue_song_pos(uint pos);
		[CCode (cname = "mpd_run_get_queue_song_pos")]
		public Song run_get_queue_song_pos(uint pos);
		[CCode (cname = "mpd_send_get_queue_song_id")]
		public bool send_get_queue_song_id(uint id);
		[CCode (cname = "mpd_run_get_queue_song_id")]
		public Song run_get_queue_song_id(uint id);
		[CCode (cname = "mpd_send_queue_changes_meta")]
		public bool send_queue_changes_meta(uint version);
		[CCode (cname = "mpd_send_queue_changes_brief")]
		public bool send_queue_changes_brief(uint version);
		[CCode (cname = "mpd_recv_queue_change_brief")]
		public bool recv_queue_change_brief(out uint position_r, out uint id_r);
		[CCode (cname = "mpd_send_add")]
		public bool send_add(string file);
		[CCode (cname = "mpd_run_add")]
		public bool run_add(string uri);
		[CCode (cname = "mpd_send_add_id")]
		public bool send_add_id(string file);
		[CCode (cname = "mpd_send_add_id_to")]
		public bool send_add_id_to(string uri, uint to);
		[CCode (cname = "mpd_recv_song_id")]
		public int recv_song_id();
		[CCode (cname = "mpd_run_add_id")]
		public int run_add_id(string file);
		[CCode (cname = "mpd_run_add_id_to")]
		public int run_add_id_to(string uri, uint to);
		[CCode (cname = "mpd_send_delete")]
		public bool send_delete(uint pos);
		[CCode (cname = "mpd_run_delete")]
		public bool run_delete(uint pos);
		[CCode (cname = "mpd_send_delete_range")]
		public bool send_delete_range(uint start, uint end);
		[CCode (cname = "mpd_run_delete_range")]
		public bool run_delete_range(uint start, uint end);
		[CCode (cname = "mpd_send_delete_id")]
		public bool send_delete_id(uint id);
		[CCode (cname = "mpd_run_delete_id")]
		public bool run_delete_id(uint id);
		[CCode (cname = "mpd_send_shuffle")]
		public bool send_shuffle();
		[CCode (cname = "mpd_run_shuffle")]
		public bool run_shuffle();
		[CCode (cname = "mpd_send_shuffle_range")]
		public bool send_shuffle_range(uint start, uint end);
		[CCode (cname = "mpd_run_shuffle_range")]
		public bool run_shuffle_range(uint start, uint end);
		[CCode (cname = "mpd_send_clear")]
		public bool send_clear();
		[CCode (cname = "mpd_run_clear")]
		public bool run_clear();
		[CCode (cname = "mpd_send_move")]
		public bool send_move(uint from, uint to);
		[CCode (cname = "mpd_run_move")]
		public bool run_move(uint from, uint to);
		[CCode (cname = "mpd_send_move_id")]
		public bool send_move_id(uint from, uint to);
		[CCode (cname = "mpd_run_move_id")]
		public bool run_move_id(uint from, uint to);
		[CCode (cname = "mpd_send_move_range")]
		public bool send_move_range(uint start, uint end, uint to);
		[CCode (cname = "mpd_run_move_range")]
		public bool run_move_range(uint start, uint end, uint to);
		[CCode (cname = "mpd_send_swap")]
		public bool send_swap(uint pos1, uint pos2);
		[CCode (cname = "mpd_run_swap")]
		public bool run_swap(uint pos1, uint pos2);
		[CCode (cname = "mpd_send_swap_id")]
		public bool send_swap_id(uint id1, uint id2);
		[CCode (cname = "mpd_run_swap_id")]
		public bool run_swap_id(uint id1, uint id2);
		[CCode (cname = "mpd_send_prio")]
		public bool send_prio(int priority, uint position);
		[CCode (cname = "mpd_run_prio")]
		public bool run_prio(int priority, uint position);
		[CCode (cname = "mpd_send_prio_range")]
		public bool send_prio_range(int priority, uint start, uint end);
		[CCode (cname = "mpd_run_prio_range")]
		public bool run_prio_range(int priority, uint start, uint end);
		[CCode (cname = "mpd_send_prio_id")]
		public bool send_prio_id(int priority, uint id);
		[CCode (cname = "mpd_run_prio_id")]
		public bool run_prio_id(int priority, uint id);
		[CCode (cname = "mpd_send_current_song")]
		public bool send_current_song();
		[CCode (cname = "mpd_run_current_song")]
		public Song run_current_song();
		[CCode (cname = "mpd_send_play")]
		public bool send_play();
		[CCode (cname = "mpd_run_play")]
		public bool run_play();
		[CCode (cname = "mpd_send_play_pos")]
		public bool send_play_pos(uint song_pos);
		[CCode (cname = "mpd_run_play_pos")]
		public bool run_play_pos(uint song_pos);
		[CCode (cname = "mpd_send_play_id")]
		public bool send_play_id(uint id);
		[CCode (cname = "mpd_run_play_id")]
		public bool run_play_id(uint song_id);
		[CCode (cname = "mpd_send_stop")]
		public bool send_stop();
		[CCode (cname = "mpd_run_stop")]
		public bool run_stop();
		[CCode (cname = "mpd_send_toggle_pause")]
		public bool send_toggle_pause();
		[CCode (cname = "mpd_run_toggle_pause")]
		public bool run_toggle_pause();
		[CCode (cname = "mpd_send_pause")]
		public bool send_pause(bool mode);
		[CCode (cname = "mpd_run_pause")]
		public bool run_pause(bool mode);
		[CCode (cname = "mpd_send_next")]
		public bool send_next();
		[CCode (cname = "mpd_run_next")]
		public bool run_next();
		[CCode (cname = "mpd_send_previous")]
		public bool send_previous();
		[CCode (cname = "mpd_run_previous")]
		public bool run_previous();
		[CCode (cname = "mpd_send_seek_pos")]
		public bool send_seek_pos(uint song_pos, uint t);
		[CCode (cname = "mpd_run_seek_pos")]
		public bool run_seek_pos(uint song_pos, uint t);
		[CCode (cname = "mpd_send_seek_id")]
		public bool send_seek_id(uint id, uint t);
		[CCode (cname = "mpd_run_seek_id")]
		public bool run_seek_id(uint song_id, uint t);
		[CCode (cname = "mpd_send_repeat")]
		public bool send_repeat(bool mode);
		[CCode (cname = "mpd_run_repeat")]
		public bool run_repeat(bool mode);
		[CCode (cname = "mpd_send_random")]
		public bool send_random(bool mode);
		[CCode (cname = "mpd_run_random")]
		public bool run_random(bool mode);
		[CCode (cname = "mpd_send_single")]
		public bool send_single(bool mode);
		[CCode (cname = "mpd_run_single")]
		public bool run_single(bool mode);
		[CCode (cname = "mpd_send_consume")]
		public bool send_consume(bool mode);
		[CCode (cname = "mpd_run_consume")]
		public bool run_consume(bool mode);
		[CCode (cname = "mpd_send_crossfade")]
		public bool send_crossfade(uint seconds);
		[CCode (cname = "mpd_run_crossfade")]
		public bool run_crossfade(uint seconds);
		[CCode (cname = "mpd_send_mixrampdb")]
		public bool send_mixrampdb(float db);
		[CCode (cname = "mpd_run_mixrampdb")]
		public bool run_mixrampdb(float db);
		[CCode (cname = "mpd_send_mixrampdelay")]
		public bool send_mixrampdelay(float seconds);
		[CCode (cname = "mpd_run_mixrampdelay")]
		public bool run_mixrampdelay(float seconds);
		[CCode (cname = "mpd_send_clearerror")]
		public bool send_clearerror();
		[CCode (cname = "mpd_run_clearerror")]
		public bool run_clearerror();
		[CCode (cname = "mpd_send_list_playlists")]
		public bool send_list_playlists();
		[CCode (cname = "mpd_send_subscribe")]
		public bool send_subscribe(string channel);
		[CCode (cname = "mpd_run_subscribe")]
		public bool run_subscribe(string channel);
		[CCode (cname = "mpd_send_unsubscribe")]
		public bool send_unsubscribe(string channel);
		[CCode (cname = "mpd_run_unsubscribe")]
		public bool run_unsubscribe(string channel);
		[CCode (cname = "mpd_send_send_message")]
		public bool send_send_message(string channel, string text);
		[CCode (cname = "mpd_run_send_message")]
		public bool run_send_message(string channel, string text);
		[CCode (cname = "mpd_send_read_messages")]
		public bool send_read_messages();
		[CCode (cname = "mpd_recv_message")]
		public Message recv_message();
		[CCode (cname = "mpd_send_channels")]
		public bool send_channels();
		[CCode (cname = "mpd_recv_channel_pair")]
		public Pair recv_channel_pair();
	}

	[CCode (cname = "struct mpd_directory",
		copy_function = "mpd_directory_dup",
		free_function = "mpd_directory_free")]
	[Compact]
	public class Directory {
		public string path { get; }

		public unowned string get_path();
		public bool feed(Mpd.Pair pair);
		public Entity? entity_begin();
	}

	[CCode (cname = "struct mpd_entity",
		free_function = "mpd_entity_free")]
	[Compact]
	public class Entity {
		public EntityType type { get; }
		public Directory directory { get; }
		public Song song { get; }
		public Playlist playlist { get; }

		public EntityType get_type();
		public Directory get_directory();
		public Song get_song();
		public Playlist get_playlist();
		public bool feed(Pair pair);
	}

	[CCode (cname = "struct mpd_output",
		free_function = "mpd_output_free")]
	[Compact]
	public class Output {
		public uint id { get; }
		public string name { get; }
		public bool enabled { get; }

		public bool feed(Pair pair);
		public uint get_id();
		public unowned string get_name();
		public bool get_enabled();
	}

	[CCode (cname = "struct mpd_pair",
		free_function = "")]
	[Compact]
	public class Pair {
		public unowned string name;
		public unowned string value;
		[CCode (cname = "mpd_directory_begin")]
		public Directory? directory_begin();
		[CCode (cname = "mpd_output_begin")]
		public Output output_begin();
		[CCode (cname = "mpd_playlist_begin")]
		public Playlist playlist_begin();
		[CCode (cname = "mpd_song_begin")]
		public Song song_begin();
		[CCode (cname = "mpd_idle_parse_pair")]
		public Idle idle_parse_pair();
	}

	[CCode (cname = "struct mpd_parser",
		free_function = "mpd_parser_free")]
	[Compact]
	public class Parser {
		public ServerError server_error { get; }
		public uint at { get; }
		public string message { get; }
		public string name { get; }
		public string value { get; }

		public ParserResult feed(string line);
		public bool is_discrete();
		public ServerError get_server_error();
		public uint get_at();
		public unowned string get_message();
		public unowned string get_name();
		public unowned string get_value();
	}

	[CCode (cname = "struct mpd_playlist",
		copy_function = "mpd_playlist_dup",
		free_function = "mpd_playlist_free")]
	[Compact]
	public class Playlist {
		public string path { get; }
		public int64 last_modified { get; }

		public string get_path();
		public int64 get_last_modified();
		public bool feed(Pair pair);
	}

	[CCode (cname = "struct mpd_song",
		copy_function = "mpd_song_dup",
		free_function = "mpd_song_free")]
	[Compact]
	public class Song {
		public string uri { get; }
		public uint duration { get; }
		public int64 last_modified { get; }
		public uint pos { get; set; }
		public uint id { get; }

		public unowned string get_uri();
		public unowned string get_tag(TagType type, uint idx = 0);
		public uint get_duration();
		public int64 get_last_modified();
		public void set_pos(uint pos);
		public uint get_pos();
		public uint get_id();
		public bool feed(Pair pair);
	}

	[CCode (cname = "struct mpd_stats",
		free_function = "mpd_stats_free")]
	[Compact]
	public class Stats {
		public uint number_of_artist { get; }
		public uint number_of_albums { get; }
		public uint number_of_songs { get; }
		public ulong uptime { get; }
		public ulong db_update_time { get; }
		public ulong play_time { get; }
		public ulong db_play_time { get; }

		public void feed(Pair pair);
		public uint get_number_of_artists();
		public uint get_number_of_albums();
		public uint get_number_of_songs();
		public ulong get_uptime();
		public ulong get_db_update_time();
		public ulong get_play_time();
		public ulong get_db_play_time();
	}

	[CCode (cname = "struct mpd_status",
		free_function = "mpd_status_free")]
	[Compact]
	public class Status {
		public int volume { get; }
		public bool repeat { get; }
		public bool random { get; }
		public bool single { get; }
		public bool consume { get; }
		public uint queue_length { get; }
		public uint queue_version { get; }
		public State state { get; }
		public uint crossfade { get; }
		public float mixrampdb { get; }
		public float mixrampdelay { get; }
		public int song_pos { get; }
		public int song_id { get; }
		public uint elapsed_time { get; }
		public uint elapsed_ms { get; }
		public uint total_time { get; }
		public uint kbit_rate { get; }
		public AudioFormat audio_format { get; }
		public uint update_id { get; }
		public string error { get; }

		public int get_volume();
		public bool get_repeat();
		public bool get_random();
		public bool get_single();
		public bool get_consume();
		public uint get_queue_length();
		public uint get_queue_version();
		public State get_state();
		public uint get_crossfade();
		public float get_mixrampdb();
		public float get_mixrampdelay();
		public int get_song_pos();
		public int get_song_id();
		public int get_next_song_pos();
		public int get_next_song_id();
		public uint get_elapsed_time();
		public uint get_elapsed_ms();
		public uint get_total_time();
		public uint get_kbit_rate();
		public AudioFormat get_audio_format();
		public uint get_update_id();
		public unowned string get_error();
	}

	[CCode (cname = "struct mpd_settings",
		free_function = "mpd_settings_free")]
	[Compact]
	public class Settings {
		public string? host { get; }
		public uint port { get; }
		public uint timeout_ms { get; }
		public string? password { get; }

		public Settings(string? host = null, uint port = 0, uint timeout_ms = 0, string? reserved = null, string? password = null);
		public string? get_host();
		public uint get_port();
		public uint get_timeout_ms();
		public string? get_password();
	}

	[CCode (cname = "struct mpd_message",
		free_function = "mpd_message_free")]
	[Compact]
	public class Message {
		public string channel { get; }
		public string text { get; }

		public bool feed(Pair pair);
		public unowned string get_channel();
		public unowned string get_text();
	}

	public Status status_begin();
	public unowned string tag_name(TagType type);
	public TagType tag_name_parse(string name);
	public TagType tag_name_iparse(string name);
	public unowned string? idle_name(Idle idle);
	public Idle idle_name_parse(string name);
	public Stats stats_begin();
	public unowned string parse_sticker(string input, out int64 name_length_r);

	public const string LIBMPDCLIENT_MAJOR_VERSION;
	public const string LIBMPDCLIENT_MINOR_VERSION;
	public const string LIBMPDCLIENT_PATCH_VERSION;
}
