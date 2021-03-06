/*订单数据配置*/
let url;
if (process.env.NODE_ENV === 'production') {
  url = window.g.API_URL
} else {
  url = window.g.LOCAL_URL
}
export const orderOperUrl = url + '/mes_server/order';
export const routerUrl = url + "/mes_server/order/select";

//export const routerUrl = "http://10.10.11.109:8080/mes_server/order/select";
export const setRouterConfig = (name) => {
  if (name === 'order_manage') {
    return {
      data: ROUTER_CONFIG.OrderManage
    };
  }
};

const ROUTER_CONFIG = {
  OrderManage: {
    queryOptions: [
      {
        id: 'ZhiDan',
        name: '制单号',
        model: '',
        type: 'text'
      }
    ],
    dataColumns: [
      {field: 'Id', title: '序号', visible: false},
      {field: 'showId', title: '序号', colStyle: {'width': '70px'}},
      {title: '操作', tdComp: 'EditOptions', colStyle: {'width': '100px'}},
      {field: 'ShowStatus', title: '状态', colStyle: {'width': '80px'}},
      {field: 'Status', title: '状态', colStyle: {'width': '80px'}, visible: false},
      {field: 'ZhiDan', title: '制单号', colStyle: {'width': '130px'}, notNull: true},
      {field: 'SoftModel', title: '型号', colStyle: {'width': '100px'}, notNull: true},
      {field: 'SN1', title: 'SN1', colStyle: {'width': '100px'}, notNull: true},
      {field: 'SN2', title: 'SN2', colStyle: {'width': '70px'}, notNull: true},
      {field: 'SN3', title: 'SN3', colStyle: {'width': '70px'}, notNull: true},
      {field: 'BoxNo1', title: '箱号1', colStyle: {'width': '70px'}, notNull: true},
      {field: 'BoxNo2', title: '箱号2', colStyle: {'width': '70px'}, notNull: true},
      {field: 'ProductDate', title: '生产日期', colStyle: {'width': '100px'}, notNull: true},
      {field: 'Color', title: '颜色', colStyle: {'width': '60px'}, notNull: true},
      {field: 'Weight', title: '重量', colStyle: {'width': '80px'}, notNull: true},
      {field: 'Qty', title: '数量', colStyle: {'width': '80px'}, notNull: true},
      {field: 'ProductNo', title: '产品编号', colStyle: {'width': '140px'}, notNull: true},
      {field: 'Version', title: '版本', colStyle: {'width': '140px'}, notNull: true},
      {field: 'IMEIStart', title: '起始IMEI号', colStyle: {'width': '150px'}, notNull: true},
      {field: 'IMEIEnd', title: '终止IMEI号', colStyle: {'width': '150px'}, notNull: true},
      {field: 'SIMStart', title: '起始SIM卡号', colStyle: {'width': '150px'}, notNull: false},
      {field: 'SIMEnd', title: '终止SIM卡号', colStyle: {'width': '150px'}, notNull: false},
      {field: 'BATStart', title: '起始BAT号', colStyle: {'width': '120px'}, notNull: false},
      {field: 'BATEnd', title: '终止BAT号', colStyle: {'width': '120px'}, notNull: false},
      {field: 'VIPStart', title: '起始VIP号', colStyle: {'width': '120px'}, notNull: false},
      {field: 'VIPEnd', title: '终止VIP号', colStyle: {'width': '120px'}, notNull: false},
      {field: 'IMEIRel', title: 'IMEI关联', colStyle: {'width': '150px'}, notNull: true},
      {field: 'TACInfo', title: 'TAC信息', colStyle: {'width': '100px'}, notNull: true},
      {field: 'CompanyName', title: '公司名', colStyle: {'width': '100px'}, notNull: false},
      {field: 'Remark1', title: '备注1', colStyle: {'width': '200px'}, notNull: false},
      {field: 'Remark2', title: '备注2', colStyle: {'width': '100px'}, notNull: false},
      {field: 'Remark3', title: '备注3', colStyle: {'width': '100px'}, notNull: false},
      {field: 'Remark4', title: '备注4', colStyle: {'width': '100px'}, notNull: false},
      {field: 'Remark5', title: '备注5', colStyle: {'width': '100px'}, notNull: false},
      {field: 'JST_template', title: 'JST模板', colStyle: {'width': '100px'}, notNull: false},
      {field: 'CHT_template1', title: 'CHT模板1', colStyle: {'width': '100px'}, notNull: false},
      {field: 'CHT_template2', title: 'CHT模板2', colStyle: {'width': '100px'}, notNull: false},
      {field: 'BAT_prefix', title: 'BAT前缀', colStyle: {'width': '100px'}, notNull: false},
      {field: 'BAT_digits', title: 'BAT位数', colStyle: {'width': '100px'}, notNull: false},
      {field: 'SIM_prefix', title: 'SIM前缀', colStyle: {'width': '100px'}, notNull: false},
      {field: 'SIM_digits', title: 'SIM位数', colStyle: {'width': '100px'}, notNull: false},
      {field: 'VIP_prefix', title: 'VIP前缀', colStyle: {'width': '100px'}, notNull: false},
      {field: 'VIP_digits', title: 'VIP位数', colStyle: {'width': '100px'}, notNull: false},
      {field: 'ICCID_prefix', title: 'ICCID前缀', colStyle: {'width': '100px'}, notNull: false},
      {field: 'ICCID_digits', title: 'ICCID位数', colStyle: {'width': '100px'}, notNull: false},
      {field: 'IMEIPrints', title: 'IMEI打印', colStyle: {'width': '100px'}, notNull: false},
      {field: 'MAC_prefix', title: 'MAC前缀', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'MAC_digits', title: 'MAC位数',colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'Equipment_prefix', title: 'Equipment前缀', colStyle: {'width': '100px'}, value: '', "notNull": false},
      {field: 'Equipment_digits', title: 'Equipment位数', colStyle: {'width': '100px'}, value: '', "notNull": false},
    ]
  },


}
/*
* [TACInfo] varchar(50) NOT NULL ,
[CompanyName] varchar(50) NULL ,
[Remark1] varchar(50) NULL ,
[Remark2] varchar(50) NULL ,
[Remark3] varchar(50) NULL ,
[Remark4] varchar(50) NULL ,
[Remark5] varchar(50) NULL ,
[Status] int NOT NULL ,
[_MASK_FROM_V2] timestamp NOT NULL ,
[JST_template] varchar(255) NULL ,
[CHT_template1] varchar(255) NULL ,
[CHT_template2] varchar(255) NULL ,
[BAT_prefix] varchar(50) NULL ,
[BAT_digits] varchar(50) NULL ,
[SIM_prefix] varchar(50) NULL ,
[SIM_digits] varchar(50) NULL ,
[VIP_prefix] varchar(50) NULL ,
[VIP_digits] varchar(50) NULL ,
[ICCID_prefix] varchar(50) NULL ,
[ICCID_digits] varchar(50) NULL ,
[IMEIPrints] varchar(255) NULL */
