define(function(require, exports, module) {
	var wrapper = require('_wrapper');
	var self = exports;
	wrapper.init(mui);
	var faultDescCn;
	self.getUserCNnameByENname = function(username) {
		var usernames = JSON.parse(localStorage.getItem('$usernames'));
		function getUserNameCn(name) {
			for(var idx in usernames) {
				if(username && (usernames[idx].username == username.trim())) {
					return usernames[idx].usernameCn;
				}
			}
		}
		return getUserNameCn(username);
	}

	self.getHeadimgurlByUsername = function(username) {
		var usernames = JSON.parse(localStorage.getItem('$usernames'));
		function getHeadimgurl(name) {
			for(var idx in usernames) {
				if(username && (usernames[idx].username == username.trim())) {
					return usernames[idx].headimgurl;
				}
			}
		}
		if(getHeadimgurl(username)) {
			if(getHeadimgurl(username).indexOf('http')!=-1) {
				return getHeadimgurl(username);
			} else {
				return wrapper.getImageDownloadUrl()+getHeadimgurl(username);
			}
		}
		return '../styles/images/icon.png';
	}

	self.getFaultDescById=function(faultDescId) {
        if (CONSTANTS.AJAX.ENABLED) {
            $.ajax({
                type: 'GET',
                url: CONSTANTS.CONF.RESOURCE_BASIC_DATA+'/faultDescriptions/'+faultDescId,
                data: {},
                dataType: 'JSON',
                async:false,
                headers: {
                    "Authorization": "bearer " + CONSTANTS.AJAX.TO_KEN
                },
                success: function (data) {
                    faultDescCn = data.nameCn;
                }
            });
        }
        return  faultDescCn;
	}
});