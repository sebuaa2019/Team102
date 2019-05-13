// 作者：陈麒先
// 时间：2019.5.8

// 添加功耗计
function addPowerGauge(){
    let oPowerGaugeGraph = document.getElementsByClassName('gauge_power')[0];
    let myPowerGaugeGraph = echarts.init(oPowerGaugeGraph,'macarons');
    // console.log(oPowerGaugeGraph);
    let powerGaugeOption = {
        backgroundColor: '#1b1b1b',
        tooltip : {
            formatter: "{a} <br/>{c} {b}"
        },
        toolbox: {
            show : false,
            feature : {
                mark : {show: true},
                restore : {show: true},
                saveAsImage : {show: true}
            }
        },
        title:{
            text: '机器人功耗计',
            textStyle: {       // 属性lineStyle控制线条样式
                fontWeight: 'bolder',
                fontSize: 16,
                fontStyle: 'normal',
                color: '#fff',
                shadowColor : '#fff', //默认透明
                shadowBlur: 10
            },
            left: 'center',
            bottom: '5%'
        },
        series : [
            {
                name:'功耗',
                type:'gauge',
                radius : '85%',
                min:0,
                max:7,
                endAngle:45,
                splitNumber:7,
                axisLine: {            // 坐标轴线
                    lineStyle: {       // 属性lineStyle控制线条样式
                        color: [[0.29, 'lime'],[0.86, '#1e90ff'],[1, '#ff4500']],
                        width: 2,
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                axisLabel: {            // 坐标轴小标记
                    textStyle: {       // 属性lineStyle控制线条样式
                        fontWeight: 'bolder',
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                axisTick: {            // 坐标轴小标记
                    length :12,        // 属性length控制线长
                    lineStyle: {       // 属性lineStyle控制线条样式
                        color: 'auto',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                splitLine: {           // 分隔线
                    length :20,         // 属性length控制线长
                    lineStyle: {       // 属性lineStyle（详见lineStyle）控制线条样式
                        width:3,
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                pointer: {
                    width:5,
                    shadowColor : '#fff', //默认透明
                    shadowBlur: 5
                },
                title : {
                    offsetCenter: [0, '-30%'],       // x, y，单位px
                    textStyle: {       // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                        fontWeight: 'bolder',
                        fontStyle: 'normal',
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                detail : {
                    //backgroundColor: 'rgba(30,144,255,0.8)',
                    // borderWidth: 1,
                    borderColor: '#fff',
                    shadowColor : '#fff', //默认透明
                    shadowBlur: 5,
                    width: 80,
                    height:30,
                    offsetCenter: [25, '20%'],       // x, y，单位px
                    textStyle: {       // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                        fontWeight: 'bolder',
                        color: '#fff',
                        fontSize:20
                    }
                },
                data:[{
                    value: 1.5, 
                    name: 'x100 KwH'
                }]
            }            
        ]
    };

    setInterval(function (){
        powerGaugeOption.series[0].data[0].value = (Math.random()*7).toFixed(2) - 0;
        // gaugeOption.series[1].data[0].value = (Math.random()*7).toFixed(2) - 0;
        // gaugeOption.series[2].data[0].value = (Math.random()*2).toFixed(2) - 0;
        // gaugeOption.series[3].data[0].value = (Math.random()*2).toFixed(2) - 0;
        myPowerGaugeGraph.setOption(powerGaugeOption);
    },2000)
}
// 添加电池与运行温度显示
function addTempNBatteryGauge(){
    let oTNBGaugeGraph = document.getElementsByClassName('gauge_tempNbattery')[0];
    let myTNBGaugeGraph = echarts.init(oTNBGaugeGraph , 'macarons');

    let TNBGaugeOption = {
        backgroundColor: '#1b1b1b',
        tooltip : {
            formatter: "{a} <br/>{c} {b}"
        },
        toolbox: {
            show : false,
            feature : {
                mark : {show: true},
                restore : {show: true},
                saveAsImage : {show: true}
            }
        },
        title:{
            text: '机器人电池与运行温度',
            textStyle: {       // 属性lineStyle控制线条样式
                fontWeight: 'bolder',
                fontSize: 16,
                fontStyle: 'normal',
                color: '#fff',
                shadowColor : '#fff', //默认透明
                shadowBlur: 10
            },
            left: 'center',
            bottom: '5%'
        },
        series : [
            {
                name:'battery',
                type:'gauge',
                radius : '65%',
                min:0,
                max:2,
                startAngle:170,
                endAngle:10,
                splitNumber:2,
                axisLine: {            // 坐标轴线
                    lineStyle: {       // 属性lineStyle控制线条样式
                        color: [[0.2, 'lime'],[0.8, '#1e90ff'],[1, '#ff4500']],
                        width: 2,
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                axisTick: {            // 坐标轴小标记
                    length :12,        // 属性length控制线长
                    lineStyle: {       // 属性lineStyle控制线条样式
                        color: 'auto',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                axisLabel: {
                    textStyle: {       // 属性lineStyle控制线条样式
                        fontWeight: 'bolder',
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    },
                    formatter:function(v){
                        switch (v + '') {
                            case '0' : return 'Full';
                            case '1' : return 'Battery';
                            case '2' : return 'Empty';
                        }
                    }
                },
                splitLine: {           // 分隔线
                    length :15,         // 属性length控制线长
                    lineStyle: {       // 属性lineStyle（详见lineStyle）控制线条样式
                        width:3,
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                pointer: {
                    width:2,
                    shadowColor : '#fff', //默认透明
                    shadowBlur: 5
                },
                title : {
                    show: false
                },
                detail : {
                    show: false
                },
                data:[{value: 0.05, name: 'battery'}]
            },
            {
                name:'temperature',
                type:'gauge',
                radius : '65%',
                min:0,
                max:2,
                startAngle:350,
                endAngle:190,
                splitNumber:2,
                axisLine: {            // 坐标轴线
                    lineStyle: {       // 属性lineStyle控制线条样式
                        color: [[0.2, 'lime'],[0.8, '#1e90ff'],[1, '#ff4500']],
                        width: 2,
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                axisTick: {            // 坐标轴小标记
                    show: false
                },
                axisLabel: {
                    textStyle: {       // 属性lineStyle控制线条样式
                        fontWeight: 'bolder',
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    },
                    formatter:function(v){
                        switch (v + '') {
                            case '0' : return 'Low';
                            case '1' : return 'Temperature';
                            case '2' : return 'High';
                        }
                    }
                },
                splitLine: {           // 分隔线
                    length :15,         // 属性length控制线长
                    lineStyle: {       // 属性lineStyle（详见lineStyle）控制线条样式
                        width:3,
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                pointer: {
                    width:2,
                    shadowColor : '#fff', //默认透明
                    shadowBlur: 5
                },
                title : {
                    show: false
                },
                detail : {
                    show: false
                },
                data:[{value: 1, name: 'temp'}]
            }           
        ]
    };

    setInterval(function (){
        TNBGaugeOption.series[0].data[0].value += 0.05;
        TNBGaugeOption.series[1].data[0].value += ((Math.random()*0.1)- 0.05);
        myTNBGaugeGraph.setOption(TNBGaugeOption);
    },2000)
}
// 添加速度计
function addGauge(){
    let oGaugeGraph = document.getElementsByClassName('gauge')[0];
    let myGaugeGraph = echarts.init(oGaugeGraph,'macarons');
    let gaugeOption = {
        backgroundColor: '#1b1b1b',
        tooltip : {
            formatter: "{a} <br/>{c} {b}"
        },
        toolbox: {
            show : false,
            feature : {
                mark : {show: true},
                restore : {show: true},
                saveAsImage : {show: true}
            }
        },
        title:{
            text: '机器人速度计',
            textStyle: {       // 属性lineStyle控制线条样式
                fontWeight: 'bolder',
                color: '#fff',
                shadowColor : '#fff', //默认透明
                shadowBlur: 10,
                fontSize: '18px',
                fontStyle: 'normal'
            },
            left: 'center',
            bottom: '5%'
        },
        series : [
            {
                name:'速度',
                type:'gauge',
                min:0,
                max:220,
                splitNumber:11,
                radius: '85%',
                axisLine: {            // 坐标轴线
                    lineStyle: {       // 属性lineStyle控制线条样式
                        color: [[0.09, 'lime'],[0.82, '#1e90ff'],[1, '#ff4500']],
                        width: 3,
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                axisLabel: {            // 坐标轴小标记
                    textStyle: {       // 属性lineStyle控制线条样式
                        fontWeight: 'bolder',
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                axisTick: {            // 坐标轴小标记
                    length :15,        // 属性length控制线长
                    lineStyle: {       // 属性lineStyle控制线条样式
                        color: 'auto',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                splitLine: {           // 分隔线
                    length :25,         // 属性length控制线长
                    lineStyle: {       // 属性lineStyle（详见lineStyle）控制线条样式
                        width:3,
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                pointer: {           // 分隔线
                    shadowColor : '#fff', //默认透明
                    shadowBlur: 5
                },
                title : {
                    textStyle: {       // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                        fontWeight: 'bolder',
                        fontSize: 20,
                        fontStyle: 'normal',
                        color: '#fff',
                        shadowColor : '#fff', //默认透明
                        shadowBlur: 10
                    }
                },
                detail : {
                    backgroundColor: 'rgba(30,144,255,0.8)',
                    borderWidth: 1,
                    borderColor: '#fff',
                    shadowColor : '#fff', //默认透明
                    shadowBlur: 5,
                    offsetCenter: [0, '50%'],       // x, y，单位px
                    textStyle: {       // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                        fontWeight: 'bolder',
                        color: '#fff',
                        fontSize: '18px'
                    }
                },
                data:[{value: 40, name: 'm/min'}]
            }            
        ]
    };
    setInterval(function (){
        gaugeOption.series[0].data[0].value = (Math.random()*100).toFixed(2) - 0;
        // gaugeOption.series[1].data[0].value = (Math.random()*7).toFixed(2) - 0;
        // gaugeOption.series[2].data[0].value = (Math.random()*2).toFixed(2) - 0;
        // gaugeOption.series[3].data[0].value = (Math.random()*2).toFixed(2) - 0;
        myGaugeGraph.setOption(gaugeOption);
    },2000)
}
// 绑定选项卡监听事件
function monitorClick(){
    var oListGroupItem = document.getElementsByClassName('list-group-item');
    console.log(oListGroupItem);
    var len = oListGroupItem.length;
    for(var i = 0 ; i < len ; i ++){
        (function(n){
            oListGroupItem[n].onclick = function(){
                for(var j = 0 ; j < len ; j ++){
                    oListGroupItem[j].className = 'list-group-item';
                }
                oListGroupItem[n].className = 'list-group-item active';
            }
        }(i))
    }
}
// 初始化函数
function init(){
    monitorClick();
    addGauge();
    addPowerGauge();
    addTempNBatteryGauge();
}
// 执行JS脚本
init();