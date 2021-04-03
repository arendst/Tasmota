libmpdclient2 {
global:
	/* mpd/async.h */
	mpd_async_new;
	mpd_async_free;
	mpd_async_get_error;
	mpd_async_get_error_message;
	mpd_async_get_system_error;
	mpd_async_get_fd;
	mpd_async_set_keepalive;
	mpd_async_events;
	mpd_async_io;
	mpd_async_send_command_v;
	mpd_async_send_command;
	mpd_async_recv_line;
	mpd_async_recv_raw;

	/* mpd/capabilities.h */
	mpd_send_allowed_commands;
	mpd_send_disallowed_commands;
	mpd_send_list_url_schemes;
	mpd_send_list_tag_types;
	mpd_send_disable_tag_types;
	mpd_run_disable_tag_types;
	mpd_send_enable_tag_types;
	mpd_run_enable_tag_types;
	mpd_send_clear_tag_types;
	mpd_run_clear_tag_types;

	/* mpd/connection.h */
	mpd_connection_new;
	mpd_connection_new_async;
	mpd_connection_free;
	mpd_connection_set_keepalive;
	mpd_connection_get_settings;
	mpd_connection_set_timeout;
	mpd_connection_get_fd;
	mpd_connection_get_async;
	mpd_connection_get_error;
	mpd_connection_get_error_message;
	mpd_connection_get_server_error;
	mpd_connection_get_server_error_location;
	mpd_connection_get_system_error;
	mpd_connection_clear_error;
	mpd_connection_get_server_version;
	mpd_connection_cmp_server_version;

	/* mpd/database.h */
	mpd_send_list_all;
	mpd_send_list_all_meta;
	mpd_send_list_meta;
	mpd_send_list_files;
	mpd_send_read_comments;
	mpd_send_update;
	mpd_send_rescan;
	mpd_recv_update_id;
	mpd_run_update;
	mpd_run_rescan;

	/* mpd/directory.h */
	mpd_directory_dup;
	mpd_directory_free;
	mpd_directory_get_path;
	mpd_directory_get_last_modified;
	mpd_directory_begin;
	mpd_directory_feed;
	mpd_recv_directory;

	/* mpd/entity.h */
	mpd_entity_free;
	mpd_entity_get_type;
	mpd_entity_get_directory;
	mpd_entity_get_song;
	mpd_entity_get_playlist;
	mpd_entity_begin;
	mpd_entity_feed;
	mpd_recv_entity;

	/* mpd/idle.h */
	mpd_idle_name;
	mpd_idle_name_parse;
	mpd_send_idle;
	mpd_send_idle_mask;
	mpd_send_noidle;
	mpd_idle_parse_pair;
	mpd_recv_idle;
	mpd_run_idle;
	mpd_run_idle_mask;
	mpd_run_noidle;

	/* mpd/list.h */
	mpd_command_list_begin;
	mpd_command_list_end;

	/* mpd/message.h */
	mpd_message_begin;
	mpd_message_feed;
	mpd_message_free;
	mpd_message_get_channel;
	mpd_message_get_text;
	mpd_send_subscribe;
	mpd_run_subscribe;
	mpd_send_unsubscribe;
	mpd_run_unsubscribe;
	mpd_send_send_message;
	mpd_run_send_message;
	mpd_send_read_messages;
	mpd_recv_message;
	mpd_send_channels;

	/* mpd/mixer.h */
	mpd_send_set_volume;
	mpd_run_set_volume;
	mpd_send_change_volume;
	mpd_run_change_volume;

	/* mpd/mount.h */
	mpd_mount_begin;
	mpd_mount_feed;
	mpd_mount_free;
	mpd_mount_get_uri;
	mpd_mount_get_storage;
	mpd_send_list_mounts;
	mpd_recv_mount;
	mpd_send_mount;
	mpd_run_mount;
	mpd_send_unmount;
	mpd_run_unmount;

	/* mpd/neighbor.h */
	mpd_neighbor_begin;
	mpd_neighbor_feed;
	mpd_neighbor_free;
	mpd_neighbor_get_uri;
	mpd_neighbor_get_display_name;
	mpd_send_list_neighbors;
	mpd_recv_neighbor;

	/* mpd/output.h */
	mpd_output_begin;
	mpd_output_feed;
	mpd_output_free;
	mpd_output_get_id;
	mpd_output_get_name;
	mpd_output_get_plugin;
	mpd_output_get_enabled;
	mpd_output_get_attribute;
	mpd_output_first_attribute;
	mpd_output_next_attribute;
	mpd_send_outputs;
	mpd_recv_output;
	mpd_send_enable_output;
	mpd_run_enable_output;
	mpd_send_disable_output;
	mpd_run_disable_output;
	mpd_send_toggle_output;
	mpd_run_toggle_output;
	mpd_send_output_set;
	mpd_run_output_set;
	mpd_send_move_output;
	mpd_run_move_output;

	/* mpd/parser.h */
	mpd_parser_new;
	mpd_parser_free;
	mpd_parser_feed;
	mpd_parser_is_discrete;
	mpd_parser_get_server_error;
	mpd_parser_get_at;
	mpd_parser_get_message;
	mpd_parser_get_name;
	mpd_parser_get_value;

	/* mpd/password.h */
	mpd_send_password;
	mpd_run_password;

	/* mpd/player.h */
	mpd_send_current_song;
	mpd_run_current_song;
	mpd_send_play;
	mpd_run_play;
	mpd_send_play_pos;
	mpd_run_play_pos;
	mpd_send_play_id;
	mpd_run_play_id;
	mpd_send_stop;
	mpd_run_stop;
	mpd_send_toggle_pause;
	mpd_run_toggle_pause;
	mpd_send_pause;
	mpd_run_pause;
	mpd_send_next;
	mpd_run_next;
	mpd_send_previous;
	mpd_run_previous;
	mpd_send_seek_pos;
	mpd_run_seek_pos;
	mpd_send_seek_id;
	mpd_run_seek_id;
	mpd_send_seek_id_float;
	mpd_run_seek_id_float;
	mpd_send_seek_current;
	mpd_run_seek_current;
	mpd_send_repeat;
	mpd_run_repeat;
	mpd_send_random;
	mpd_run_random;
	mpd_send_single_state;
	mpd_run_single_state;
	mpd_send_single;
	mpd_run_single;
	mpd_send_consume;
	mpd_run_consume;
	mpd_send_crossfade;
	mpd_run_crossfade;
	mpd_send_mixrampdb;
	mpd_run_mixrampdb;
	mpd_send_mixrampdelay;
	mpd_run_mixrampdelay;
	mpd_send_clearerror;
	mpd_run_clearerror;

	/* mpd/playlist.h */
	mpd_playlist_free;
	mpd_playlist_dup;
	mpd_playlist_get_path;
	mpd_playlist_get_last_modified;
	mpd_playlist_begin;
	mpd_playlist_feed;
	mpd_send_list_playlists;
	mpd_recv_playlist;
	mpd_send_list_playlist;
	mpd_send_list_playlist_meta;
	mpd_send_playlist_clear;
	mpd_run_playlist_clear;
	mpd_send_playlist_add;
	mpd_run_playlist_add;
	mpd_send_playlist_move;
	mpd_send_playlist_delete;
	mpd_run_playlist_delete;
	mpd_send_save;
	mpd_run_save;
	mpd_send_load;
	mpd_run_load;
	mpd_send_load_range;
	mpd_run_load_range;
	mpd_send_rename;
	mpd_run_rename;
	mpd_send_rm;
	mpd_run_rm;

	/* mpd/queue.h */
	mpd_send_list_queue_meta;
	mpd_send_list_queue_range_meta;
	mpd_send_get_queue_song_pos;
	mpd_run_get_queue_song_pos;
	mpd_send_get_queue_song_id;
	mpd_run_get_queue_song_id;
	mpd_send_queue_changes_meta;
	mpd_send_queue_changes_meta_range;
	mpd_send_queue_changes_brief;
	mpd_send_queue_changes_brief_range;
	mpd_recv_queue_change_brief;
	mpd_send_add;
	mpd_run_add;
	mpd_send_add_id;
	mpd_send_add_id_to;
	mpd_recv_song_id;
	mpd_run_add_id;
	mpd_run_add_id_to;
	mpd_send_delete;
	mpd_run_delete;
	mpd_send_delete_range;
	mpd_run_delete_range;
	mpd_send_delete_id;
	mpd_run_delete_id;
	mpd_send_shuffle;
	mpd_run_shuffle;
	mpd_send_shuffle_range;
	mpd_run_shuffle_range;
	mpd_send_clear;
	mpd_run_clear;
	mpd_send_move;
	mpd_run_move;
	mpd_send_move_id;
	mpd_run_move_id;
	mpd_send_move_range;
	mpd_run_move_range;
	mpd_send_swap;
	mpd_run_swap;
	mpd_send_swap_id;
	mpd_run_swap_id;
	mpd_send_add_tag_id;
	mpd_run_add_tag_id;
	mpd_send_clear_tag_id;
	mpd_run_clear_tag_id;
	mpd_send_clear_all_tags_id;
	mpd_run_clear_all_tags_id;
	mpd_send_prio;
	mpd_run_prio;
	mpd_send_prio_range;
	mpd_run_prio_range;
	mpd_send_prio_id;
	mpd_run_prio_id;

	/* mpd/recv.h */
	mpd_recv_pair;
	mpd_recv_pair_named;
	mpd_return_pair;
	mpd_enqueue_pair;
	mpd_recv_binary;

	/* mpd/response.h */
	mpd_response_finish;
	mpd_response_next;

	/* mpd/search.h */
	mpd_search_db_songs;
	mpd_search_add_db_songs;
	mpd_search_add_db_songs_to_playlist;
	mpd_search_queue_songs;
	mpd_search_db_tags;
	mpd_count_db_songs;
	mpd_search_add_base_constraint;
	mpd_search_add_uri_constraint;
	mpd_search_add_tag_constraint;
	mpd_search_add_any_tag_constraint;
	mpd_search_add_modified_since_constraint;
	mpd_search_add_expression;
	mpd_search_add_group_tag;
	mpd_search_add_sort_name;
	mpd_search_add_sort_tag;
	mpd_search_add_window;
	mpd_search_commit;
	mpd_search_cancel;
	mpd_recv_pair_tag;

	/* mpd/send.h */
	mpd_send_command;

	/* mpd/song.h */
	mpd_song_free;
	mpd_song_dup;
	mpd_song_get_uri;
	mpd_song_get_tag;
	mpd_song_get_duration;
	mpd_song_get_duration_ms;
	mpd_song_get_start;
	mpd_song_get_end;
	mpd_song_get_last_modified;
	mpd_song_set_pos;
	mpd_song_get_pos;
	mpd_song_get_id;
	mpd_song_get_prio;
	mpd_song_get_audio_format;
	mpd_song_begin;
	mpd_song_feed;
	mpd_recv_song;

	/* mpd/stats.h */
	mpd_send_stats;
	mpd_stats_begin;
	mpd_stats_feed;
	mpd_recv_stats;
	mpd_run_stats;
	mpd_stats_free;
	mpd_stats_get_number_of_artists;
	mpd_stats_get_number_of_albums;
	mpd_stats_get_number_of_songs;
	mpd_stats_get_uptime;
	mpd_stats_get_db_update_time;
	mpd_stats_get_play_time;
	mpd_stats_get_db_play_time;

	/* mpd/status.h */
	mpd_status_begin;
	mpd_status_feed;
	mpd_status_free;
	mpd_send_status;
	mpd_recv_status;
	mpd_run_status;
	mpd_status_get_volume;
	mpd_status_get_repeat;
	mpd_status_get_random;
	mpd_status_get_single_state;
	mpd_status_get_single;
	mpd_status_get_consume;
	mpd_status_get_queue_length;
	mpd_status_get_queue_version;
	mpd_status_get_state;
	mpd_status_get_crossfade;
	mpd_status_get_mixrampdb;
	mpd_status_get_mixrampdelay;
	mpd_status_get_song_pos;
	mpd_status_get_song_id;
	mpd_status_get_next_song_pos;
	mpd_status_get_next_song_id;
	mpd_status_get_elapsed_time;
	mpd_status_get_elapsed_ms;
	mpd_status_get_total_time;
	mpd_status_get_kbit_rate;
	mpd_status_get_audio_format;
	mpd_status_get_update_id;
	mpd_status_get_partition;
	mpd_status_get_error;

	/* mpd/tag.h */
	mpd_tag_name;
	mpd_tag_name_parse;
	mpd_tag_name_iparse;

	/* mpd/sticker.h */
	mpd_send_sticker_set;
	mpd_run_sticker_set;
	mpd_send_sticker_delete;
	mpd_run_sticker_delete;
	mpd_send_sticker_get;
	mpd_send_sticker_list;
	mpd_send_sticker_find;
	mpd_parse_sticker;
	mpd_recv_sticker;
	mpd_return_sticker;

	/* mpd/fingerprint.h */
	mpd_parse_fingerprint_type;
	mpd_send_getfingerprint;
	mpd_run_getfingerprint_chromaprint;

	/* mpd/partition.h */
	mpd_partition_new;
	mpd_partition_free;
	mpd_partition_get_name;
	mpd_send_newpartition;
	mpd_run_newpartition;
	mpd_send_delete_partition;
	mpd_run_delete_partition;
	mpd_send_switch_partition;
	mpd_run_switch_partition;
	mpd_send_listpartitions;
	mpd_recv_partition;
	mpd_send_move_output;
	mpd_run_move_output;

	/* mpd/settings.h */
	mpd_settings_new;
	mpd_settings_free;
	mpd_settings_get_host;
	mpd_settings_get_port;
	mpd_settings_get_timeout_ms;
	mpd_settings_get_password;

local:
	*;
};
