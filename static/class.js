var Class = (function() {


Class = function (parent, prot) {
    if (arguments.length == 1)
    {
        prot = parent;
        parent = undefined;
    }


    var Constructor = function() {
        if (this.init)
            return this.init.apply(this, arguments);
        else
            return this;
    }

    ctor = Constructor;

    if (parent)
    {
        ctor.prototype = Object.create(parent.prototype);
        ctor.superclass = parent.prototype;
    }
    else
        ctor.prototype = {};


    for (var i in Class.class_methods)
        ctor[i] = Class.class_methods[i];

    ctor.implement(Class.common_methods);
    ctor.implement(prot);


    ctor.prototype.constructor = ctor;

    return ctor;
};

Class.class_methods = {
    implement: function (iface) {
        for (var name in iface) {
            this.prototype[name] = iface[name];
            if (typeof this.prototype[name] == 'function')
            {
                this.prototype[name].$class = this;
                this.prototype[name].$name = name;
            }
        }

        return this;
    }
};


Class.common_methods = {
    base: function() {
        var caller = arguments.callee.caller;

        if (! caller.$class)
            throw new Error('Class.base() method can only be called inside method of the class');
        if (! caller.$class.superclass)
            throw new Error('No superclass');

        return caller.$class.superclass[arguments.callee.caller.$name].apply(this, arguments);
    }
};


Class.Factory = function (klass, args) {
    function F() {
        return klass.apply(this, args);
    }
    F.prototype = klass.prototype;

    return function () {
        return new F();
    };
};



var Binding = Class({
    init: function (obj, handlers) {
        this.obj = obj;
        this.handlers = handlers;

        if (! obj.$listeners) obj.$listeners = {};

        for (var i in this.handlers)
        {
            if (obj.$listeners.hasOwnProperty(i))
                obj.$listeners[i].push(this.handlers[i]);
            else
                obj.$listeners[i] = [this.handlers[i]];
        }
    },

    unbind: function () {
        for (var i in this.handlers)
            this.obj.unbind(i, this.handlers[i]);

        delete this.obj;
        delete this.handlers;
    }
});

Class.EventSource = {
    bind: function () {
        if (arguments.length == 2)
        {
            var obj = {};
            obj[arguments[0]] = arguments[1];
            return this.bind(obj);
        }

        return new Binding(this, arguments[0]);
    },

    unbind: function (eventname, callback) {
        for (var i in this.$listeners[eventname])
            if (this.$listeners[eventname][i] == callback)
                this.$listeners[eventname].splice(i, 1);
    },

    unbindAll: function () {
        delete this.$listeners;
    },

    notify: function (eventname, params) {
        if (this.$listeners)
            for (var i in this.$listeners[eventname])
                this.$listeners[eventname][i].call(this, params);

        // May be we will return something more interesting in future
        // for example, result of first/last listener
        return null;
    },

    hasListeners: function (eventname) {
        return this.$listeners && this.$listeners[eventname] && this.$listeners[eventname].length > 0;
    },

    notifier: function (eventname) {
        return (function (params) { return this.notify(eventname, params) }).bind(this);
    },

    chain: function (eventname, other, otherevent) {
        this.bind(eventname, other.notifier(otherevent ? otherevent : eventname));
        return this;
    }
};



Class.Properties = function(props) {
    var mixin = {
        get: function (propname) {
            return this[propname]();
        },

        set: function () {
            if (arguments.length == 1)
            {
                for (var i in arguments[0])
                    if (this[i])
                        this[i](arguments[0][i]);
            }
            else
                if (this[arguments[0]])
                    this[arguments[0]].apply(this, Array.prototype.slice.call(arguments, 1));
        }
    }

    for (var name in props) {
        mixin[name] = (function (name, params) {
            var prop_storage = '_' + name;

            if (params.hasOwnProperty('value'))
                mixin[prop_storage] = params.value;

            return function () {
                if (arguments.length == 0)
                {
                    if (params.get)
                        return params.get.apply(this);
                    else
                        return this[prop_storage];
                }
                else
                {
                    if (params.set)
                        params.set.apply(this, arguments);
                    else
                        this[prop_storage] = arguments[0];

                    if (params.event)
                        this.notify(params.event, arguments[0])

                    return this;
                }
            };
        })(name, props[name]);
    }

    return mixin;
}

return Class;

})();
