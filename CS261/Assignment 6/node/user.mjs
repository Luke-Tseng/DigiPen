export function CreateProfile(obj) {
    return {
    	username: obj.username,
		password: obj.password,
		avatar: obj.avatar
};
}
export function PublicProfile(user) {
    return {
        username: user.username,
        avatar: user.avatar,
        id: user._id.toString(),
    };
}

export function FullProfile(user) {
    return {
        password: user.password,
        username: user.username,
        avatar: user.avatar,
        id: user._id.toString(),
    };
}
