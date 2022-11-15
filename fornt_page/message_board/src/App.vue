<template>
  <br>
    <p>
      昵称<br>
      <input type="text" ref="name" required minlength="1" maxlength="10" size="10">
    </p>
    <p>
      留言<br>
      <textarea ref="message" rows="3" cols="25"></textarea>
    </p>
    <p>
      <button @click="commit()">提交</button>
    </p>

  <p>
    <li v-for="(item, index) in info">
      {{ item.name }}<br>
      &nbsp;&nbsp;&nbsp;# {{ item.message }}<br>
      &nbsp;&nbsp;&nbsp;# {{index}} 楼 # {{item.time}}
    </li>
  </p>
</template>

<script lang="ts">
export default {
  data: function() {
    return {
      url: String,
      port: Number,
      info: []
    }
  },
  methods: {
    async commit(){
      const name = this.$refs.name.value
      const message = this.$refs.message.value
      if(!name || !message) {
        console.debug("name or message is null. dont commit.")
        return
      }
      const time = new Date().toLocaleString()
      await fetch(`${this.url}:${this.port}` + "/message_board",{
        method: 'POST',
        body: JSON.stringify({
          "name": name,
          "message": message,
          "time": time
        })
      })
      window.location.reload()
    }
  },
  async mounted() {
    console.log('load app vue')
    this.url = "http://da1234cao.tk" // 这里是资源服务器的地址和端口
    this.port = 9999
    const data = await fetch(`${this.url}:${this.port}` + "/message_board/all",{
        method: 'GET',
      }).then(resp =>{
        return resp.json()
      }).then(resp => {
        this.info = resp.info
      })
  }
}
</script>

<style scoped>
</style>
