<<<<<<< Demon Castle - Last Fight
Tên game: Demon Castle - Last Fight

Lối chơi, logic của game: Người chơi vào vai anh hùng được triệu hồi tới từ thế giới khác. Sau khi trải qua khoảng thời gian tập luyện cũng như tiêu diệt các đợt quái vật khắp nơi trên thế giới, hạ được hầu hết các trụ cột của quân ma vương thì giờ đây anh hùng sẽ tiến tới căn cứ chính, là lâu đài của ma vương và tiêu diệt hắn (đoạn mở đầu game sẽ cho ta biết thêm, nhớ đọc hết). Game sẽ cho ta thám hiểm bên trong lâu đài, thám thính để hiểu địa hình trước khi tiêu diệt ma vương. (còn phần combat ma vương sẽ ở phần 2 của game, mà đó là bài khác chứ bài này chỉ là thám hiểm thôi :V)

Đồ họa, âm thanh: Màn hình tỉ lệ 720x480 có thể tùy chỉnh kích thước, nhưng khuyến cáo nên toàn màn hình chứ để dọc ngang quá sẽ gây lỗi render game (khắc phục thì đặt lại tỉ lệ là được), thư viện đồ họa & âm thanh SDL2.

Tóm tắt các scene hay các chặng của game:

- Đoạn đầu là trong sảnh nơi anh hùng mới tiến vào lâu đài ma vương, người chơi có thể đi xung quanh, vào các phòng để lấy vật phẩm thêm (nhưng hiện khóa hết cửa rồi), tiến tới tầng 2 và tìm đường đi tiếp, chủ yếu là tham quan.
- Đoạn sau sẽ cho ta tới 1 dạng mê cung dịch chuyển, người chơi phải tìm đúng đường để xuống được bên dưới.
- Bên dưới sẽ có 4 cổng dịch chuyển và cũng là 4 màn chơi thử thách đầu tiên.
+ Màn 1 là giải mê cung (bình thường nhưng sẽ mất khá lâu thời gian dù có dùng cách đi liên tục sát bên phải hay trái mê cung).
+ Màn 2 là giải đố toán lí hóa, vì muốn thấy sự bất lực và yếu kém của người chơi khi gặp 3 câu đố mỗi lĩnh vực nên tôi tạo ra nó. Hầu hết sẽ chụp gửi AI để qua màn cho nhanh nhưng tôi sẽ tính trước nó.
+ Màn 3, ở đây cách di chuyển sẽ được chuyển sang dạng khác đa dạng và linh hoạt hơn vì từ đây sẽ không đơn giản nữa. Màn 3 sẽ là né đường đạn vì đi tới điểm cuối cùng để lấy viên đá.
+ Màn 4 là chiến đấu với boss chính của ss1. Là ai thì chơi là biết.

Cấu trúc của project game: 

Folder core:

+ main.cpp: quản lí chính, là nơi hoạt động game.
+ Scene.h: nơi quản lí các scene của game, chứa lượng code dài và nhiều nhất vì tất cả code sẽ nằm tại đây, sau đó mới đóng gói và đưa vào main cho file main ít nhất.
+ Texture.h: là file lưu giữ class Texture để xử lí các tác vụ liên quan đến Texture và render.
+ Button.h: là class tương tự Texture, nhưng tác vụ chính là render các nút để thực thi các sự kiện ấn nút hay gì đó tương tự, ta sẽ truyền biến Texture vào để nó nhận các dữ liệu của biến đó.
+ Timer.h: là file kiểm soát thời gian trong game, sử dụng để chạy các time khác nhau trong game mà không bị ảnh hưởng tới nhau,  giúp các vật thể hoạt động độc lập hơn.
+ Window.h: xử lí các nhiệm vụ liên quan đến màn hình, như chỉnh kích thước hay bật tắt VSYNC.
+ Character.h: xử lí các tác vụ liên quan đến các nhân vật trong game, bao gồm anh hùng và boss.
+ rangedM.h: xử lý các tác vụ liên quan đến đạn hay phép được bắn ra, bao gồm render đạn, xử lí va chạm và gây thiệt hại. Hiện có 9 loại đạn khác nhau, hiệu ứng khác nhau. Có thể ấn M hoặc dùng chuột ấn để tấn công. Đạn có thể khử đạn, có thể dùng để né đạn.

Folder assets:

+ File font: lưu trữ các font trong game.
+ File sound: lưu trữ các sound trong game như hiệu ứng và nhạc nền
+ File texture: bao gồm map, các ảnh, hiệu ứng, nhân vật.
+ Folder save: dùng để lưu dữ liệu game vào (bao gồm tọa độ, chỉ số, scene,...)
