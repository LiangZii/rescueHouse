const Mock = require('mockjs')

const data = Mock.mock({
  'items|10': [{
    rescueId: '@id',
    city: '@city',
    'longitude|40-42.6':40.08605,
    'latitude|114-118.6':116.479604,
    'warning|1-9':1,
    'temperature|10-20':10,
    'humidity|10-20':10,
    'light|10-20':10,
    'pm|10-20.1':10.1,
    'battery|90-100':90,
  }]
})

module.exports = [
  {
    url: '/vue-admin-template/table/list',
    type: 'get',
    response: config => {
      const items = data.items
      return {
        code: 200,
        data: {
          total: items.length,
          items: items
        }
      }
    }
  }
]
