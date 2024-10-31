function confirmLogout() {
// Hiển thị hộp thoại xác nhận
    var confirmLogout = confirm("Bạn có chắc chắn muốn đăng xuất?");

    // Nếu người dùng đồng ý
    if (confirmLogout) {
        // Chuyển hướng đến trang trangchu.html
        window.location.href = "trangchu.html";
    }
}

var nhietDo = document.getElementById('nhietdo');
var dbRefNhietDo = firebase.database().ref('Weather').child('Temperature'); 
var doAm = document.getElementById('doam');
var dbRefDoAm = firebase.database().ref('Weather').child('Humidity');  
var anhSang = document.getElementById('anhsang');
var dbRefAnhSang = firebase.database().ref('Weather').child('Light');  
var gas = document.getElementById('khigas');
var dbRefGas = firebase.database().ref('Weather').child('Gas');  
var mua = document.getElementById('mua');
var dbRefMua = firebase.database().ref('Weather').child('Rain');   

var dbRefCheMua = firebase.database().ref('Device').child('CheMua');   
var cheMua = 0;
dbRefCheMua.on('value', snap => {cheMua.innerText = snap.val()});

dbRefNhietDo.on('value', snap => {nhietDo.innerText = snap.val() + '°C'});
dbRefDoAm.on('value', snap => {doAm.innerText = snap.val() + '%'});
dbRefAnhSang.on('value', snap => {anhSang.innerText = snap.val() + '%'});

dbRefGas.on('value', snap => {gas.innerText = snap.val() + '%'});
dbRefMua.on('value', snap => {mua.innerText = snap.val() + '%'});

dbRefNhietDo.on('value', function(getdata1){
    var tem1 = getdata1.val();
    dbRefDoAm.on('value', function(getdata2){
        var tem2 = getdata2.val();
        dbRefAnhSang.on('value', function(getdata3){
            var tem3 = getdata3.val();
            dbRefGas.on('value', function(getdata4){
                var tem4 = getdata4.val();
                dbRefMua.on('value', function(getdata5){
                    var tem5 = getdata5.val();
                    updateChart(tem1, tem2, tem3,tem4,tem5);
                })
            })
        })
    })
});



var chart;
function updateChart(tem1, tem2, tem3, tem4, tem5) {
const time = new Date().toLocaleTimeString(); // Lấy thời gian hiện tại

    // Kiểm tra nếu biểu đồ đã được khởi tạo
    if (!chart) {
        // Nếu chưa, khởi tạo biểu đồ mới
        chart = new Chart("myChart_temp", {
        type: "line", // Chọn loại biểu đồ là biểu đồ đường
        data: {
            labels: [time], // Sử dụng thời gian làm nhãn trục x
            datasets: [{
            label: 'Temperature',
            borderColor: 'aqua',
            data: [tem1]
            }, {
            label: 'Humidity',
            borderColor: 'yellow',
            data: [tem2]
            }, {
            label: 'Light',
            borderColor: 'violet',
            data: [tem3]
            }, {
            label: 'Gas',
            borderColor: 'blue',
            data: [tem4]
            }, {
            label: 'Rain',
            borderColor: 'lightgreen',
            data: [tem5]
            }]
        },
        options: {
            scales: {
            x: {
                type: 'time', // Sử dụng kiểu dữ liệu thời gian cho trục x
                time: {
                unit: 'minute' // Đơn vị thời gian là phút
                }
            }
            },
            legend: {
                labels: {
                    fontColor: "white",
                    fontSize: 13,
                     display: true
                    }
            },
            title: {
            display: true,
            fontColor: "white",
            fontSize: 20,
            text: "Weather Chart"
            }
        }
        });
    } else {
        // Nếu biểu đồ đã tồn tại, cập nhật dữ liệu và vẽ lại biểu đồ
        chart.data.labels.push(time);
        chart.data.datasets[0].data.push(tem1);
        chart.data.datasets[1].data.push(tem2);
        chart.data.datasets[2].data.push(tem3);
        chart.data.datasets[3].data.push(tem4);
        chart.data.datasets[4].data.push(tem5);

        // Giới hạn số lượng điểm trên biểu đồ để tránh quá tải trình duyệt
        const maxDataPoints = 20;
        if (chart.data.labels.length > maxDataPoints) {
        chart.data.labels.shift();
        chart.data.datasets.forEach(dataset => {
            dataset.data.shift();
        });
        }

        chart.update(); // Vẽ lại biểu đồ
    }
}




const p_toggleSwitch3 = document.getElementById('p-toggleSwitch3');
const p_statusText3 = document.getElementById('p-statusText3');

// Lắng nghe sự kiện thay đổi từ toggle switch cho phòng 3
p_toggleSwitch3.addEventListener('click', function() {
    if (this.checked == false) {    
        dbRefCheMua.set(false);
        p_statusText3.textContent = "Trạng thái: Đóng"; // Thay đổi văn bản trạng thái
    } else {
        dbRefCheMua.set(true);
        p_statusText3.textContent = "Trạng thái: Mở"; // Thay đổi văn bản trạng thái
    }
});

dbRefCheMua.on('value', function(snapshot) {
    var muaValue = snapshot.val();
    
    // Cập nhật trạng thái của toggle switch và văn bản trạng thái tùy thuộc vào giá trị từ Firebase
    p_toggleSwitch3.checked = muaValue;
    p_statusText3.textContent = muaValue ? "Trạng thái: Mở" : "Trạng thái: Đóng";
});


dbRefMua.on('value', function(snapshot) {
    // Lấy giá trị của dbRefMua
    var muaValue = snapshot.val();
    // Kiểm tra nếu giá trị của mua lớn hơn 30, đặt p_toggleSwitch3 thành true, ngược lại đặt thành false
    if (muaValue > 30) {
        p_toggleSwitch3.checked = false;
        dbRefCheMua.set(false);
        p_statusText3.textContent = "Trạng thái: Đóng"; // Thay đổi văn bản trạng thái
    }
});
