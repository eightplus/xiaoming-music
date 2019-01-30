# xiaoming-music

https://mpv.io/manual/stable/#c-plugins

https://github.com/mpv-player

https://github.com/mpv-player/mpv-examples/tree/master/cplugins


###
酷狗获取歌词


Get方式:
Step 1: 查找歌曲(需要进行URL编码)
http://lyrics.kugou.com/search?ver=1&man=yes&client=pc&keyword=[歌手-歌名]&duration=[歌曲时长,单位为毫秒]&hash=

Step 2：查找歌词(从Step 1中的结果取出 candidates 部分，从中找到id和accesskey。fmt=lrc或fmt=krc)
http://lyrics.kugou.com/download?ver=1&client=pc&id=[上一步得到的id]&accesskey=[上一步得到的accesskey]&fmt=krc&charset=utf8

example:
Step 1: http://lyrics.kugou.com/search?ver=1&man=yes&client=pc&keyword=邓丽君-给我一个吻&duration=151301&hash=
Step 2:http://lyrics.kugou.com/download?ver=1&client=pc&id=16857921&accesskey=914FCBE7667BDD8042910D80EC81D094&fmt=krc&charset=utf8



###
酷狗搜索歌词

Get方式(hash必须 其他参数可选 网页端播放音乐的url就有hash): 
example: http://krcs.kugou.com/search?ver=1&man=yes&client=mobi&keyword=&duration=&hash=F3EA661A19E9A0C965AD64049040BBAC&album_audio_id=




###网易云音乐获取歌词

Step 1: post
url: http://music.163.com/api/search/pc
params: s=[歌名]&offset=0&limit=5&type=1

url: http://music.163.com/api/search/pc
params: s=[歌名][专辑名]&offset=0&limit=5&type=1

example:
url: http://music.163.com/api/search/pc
params: s=High歌&offset=0&limit=5&type=1
http://music.163.com/api/search/pc/?s=High歌&offset=0&limit=10&type=1
http://music.163.com/api/search/get/?s=刘维&薛之谦-婚礼的祝福&limit=10&type=1&offset=0

url: http://music.163.com/api/search/pc
params: s=High歌特别&offset=0&limit=5&type=1

Step 2:
获取专辑封面：
http://p2.music.126.net/G0XS2h3QquIeN0PtgjPcMw==/109951163099000028.jpg
获取歌词：
http://music.163.com/api/song/lyric?os=pc&id=%1&lv=-1&kv=-1&tv=-1

example:
http://music.163.com/api/search/pc/s=High歌&offset=0&limit=5&type=1
http://music.163.com/api/song/lyric?os=pc&id=526904655&lv=-1&kv=-1&tv=-1
http://music.163.com/api/song/lyric?os=pc&id=526904655&lv=-1&kv=-1&tv=-1
