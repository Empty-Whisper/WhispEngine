return {

    GameObject = {
        type = "lib",
        description = "GameObject class",
        childs = {
            Instantiate = {
                type = "function",
                description = "Creates an instance of a Prefab",
                args = "(Prefab: prefab)",
                returns = "(GameObject*)"
            },
            Destroy = {
                type = "function",
                description = "Destroys a GameObject and its children",
                args = "(GameObject*: gameobject)"
            },

            active = {type = "value"},
            name = {type = "value"},
            transform = {
                type = "class",
                description = "transform class",
                childs = {
                    gameobject = {type = "value"},
                    parent = {type = "value"},
                    position = {type = "value"},
                    gposition = {type = "value"},
                    rotation = {type = "value"},
                    grotation = {type = "value"},
                    scale = {type = "value"},
                    forward = {type = "value"},
                    up = {type = "value"},
                    right = {type = "value"},

                    SetPositionv = {
                        type = "method",
                        description = "Set the position given a Vector3",
                        args = "(Vector3: position)"
                    },
                    SetPosition3f = {
                        type = "method",
                        description = "Set the position given a Vector3",
                        args = "(x, y, z)"
                    },
                    SetRotation = {
                        type = "method",
                        description = "Set the rotation given a Quaternion",
                        args = "(Quaternion: rotation)"
                    },
                    SetScale3f = {
                        type = "method",
                        description = "Set the scale given a Vector3",
                        args = "(x, y, z)"
                    },
                    LookAt = {
                        type = "method",
                        description = "Rotates GameObject to look to given position",
                        args = "(Vector3: position)"
                    },
                    Find = {
                        type = "method",
                        description = "Search a GameObject with name (only in children, not iterative)",
                        args = "(string: name)",
                        returns = "(GameObject*)"
                    },
                    GetChild = {
                        type = "method",
                        description = "Search a GameObject with index n (only in children, not iterative)",
                        args = "(number: n)",
                        returns = "(GameObject*)"
                    },
                    ChildCount = {
                        type = "method",
                        description = "Returns the number of children",
                        args = "()",
                        returns = "(number)"
                    },
                    IsChilldOf = {
                        type = "method",
                        description = "Returns true if gameobject is child of given GameObject",
                        args = "(GameObject*: parent)",
                        returns = "(boolean)"
                    },
                    SetParent = {
                        type = "method",
                        description = "Change self gameobject to given parent",
                        args = "(GameObject*: parent)"
                    }
                }
            }
        }
    },

    gameObject = {
        type = "lib",
        description = "GameObject class",
        childs = {
            active = {type = "value"},
            name = {type = "value"},
            transform = {
                type = "class",
                description = "transform class",
                childs = {
                    gameobject = {type = "value"},
                    parent = {type = "value"},
                    position = {type = "value"},
                    gposition = {type = "value"},
                    rotation = {type = "value"},
                    grotation = {type = "value"},
                    scale = {type = "value"},
                    forward = {type = "value"},
                    up = {type = "value"},
                    right = {type = "value"},

                    SetPositionv = {
                        type = "method",
                        description = "Set the position given a Vector3",
                        args = "(Vector3: position)"
                    },
                    SetPosition3f = {
                        type = "method",
                        description = "Set the position given a Vector3",
                        args = "(x, y, z)"
                    },
                    SetRotation = {
                        type = "method",
                        description = "Set the rotation given a Quaternion",
                        args = "(Quaternion: rotation)"
                    },
                    SetScale3f = {
                        type = "method",
                        description = "Set the scale given a Vector3",
                        args = "(x, y, z)"
                    },
                    LookAt = {
                        type = "method",
                        description = "Rotates GameObject to look to given position",
                        args = "(Vector3: position)"
                    },
                    Find = {
                        type = "method",
                        description = "Search a GameObject with name (only in children, not iterative)",
                        args = "(string: name)",
                        returns = "(GameObject*)"
                    },
                    GetChild = {
                        type = "method",
                        description = "Search a GameObject with index n (only in children, not iterative)",
                        args = "(number: n)",
                        returns = "(GameObject*)"
                    },
                    ChildCount = {
                        type = "method",
                        description = "Returns the number of children",
                        args = "()",
                        returns = "(number)"
                    },
                    IsChilldOf = {
                        type = "method",
                        description = "Returns true if gameobject is child of given GameObject",
                        args = "(GameObject*: parent)",
                        returns = "(boolean)"
                    },
                    SetParent = {
                        type = "method",
                        description = "Change self gameobject to given parent",
                        args = "(GameObject*: parent)"
                    }
                }
            }
        }
    },

    transform = {
        type = "lib",
        description = "transform class",
        childs = {
            gameobject = {type = "value"},
            parent = {type = "value"},
            position = {type = "value"},
            gposition = {type = "value"},
            rotation = {type = "value"},
            grotation = {type = "value"},
            scale = {type = "value"},
            forward = {type = "value"},
            up = {type = "value"},
            right = {type = "value"},

            SetPositionv = {
                type = "method",
                description = "Set the position given a Vector3",
                args = "(Vector3: position)"
            },
            SetPosition3f = {
                type = "method",
                description = "Set the position given a Vector3",
                args = "(x, y, z)"
            },
            SetRotation = {
                type = "method",
                description = "Set the rotation given a Quaternion",
                args = "(Quaternion: rotation)"
            },
            SetScale3f = {
                type = "method",
                description = "Set the scale given a Vector3",
                args = "(x, y, z)"
            },
            LookAt = {
                type = "method",
                description = "Rotates GameObject to look to given position",
                args = "(Vector3: position)"
            },
            Find = {
                type = "method",
                description = "Search a GameObject with name (only in children, not iterative)",
                args = "(string: name)",
                returns = "(GameObject*)"
            },
            GetChild = {
                type = "method",
                description = "Search a GameObject with index n (only in children, not iterative)",
                args = "(number: n)",
                returns = "(GameObject*)"
            },
            ChildCount = {
                type = "method",
                description = "Returns the number of children",
                args = "()",
                returns = "(number)"
            },
            IsChilldOf = {
                type = "method",
                description = "Returns true if gameobject is child of given GameObject",
                args = "(GameObject*: parent)",
                returns = "(boolean)"
            },
            SetParent = {
                type = "method",
                description = "Change self gameobject to given parent",
                args = "(GameObject*: parent)"
            }
        }
    },

    Vector3 = {
        type = "class",
        description = "Vector3 class. Constructors (x, y, z) and (Vector3)",
        childs = {
            x = {type = "value"},
            y = {type = "value"},
            z = {type = "value"},

            Normalize = {
                type = "method",
                description = "Normalize the vector",
                args = "()",
                returns = "(number)"
            },
            Normalized = {
                type = "method",
                description = "Return the normalized vector",
                args = "()",
                returns = "(Vector3)"
            },
            Magnitude = {
                type = "method",
                description = "Return the magnitude",
                args = "()",
                returns = "(number)"
            },
            sqrMagnitude = {
                type = "method",
                description = "Return the magnitude sqrt",
                args = "()",
                returns = "(number)"
            },
            toString = {
                type = "method",
                description = "Return a string with the values",
                args = "()",
                returns = "(string)"
            },
            FromEuler = {
                type = "function",
                description = "(Static, call it from namespace)Return a quaternion with the rotation of the vector",
                args = "(x, y, z)",
                returns = "Static (Quat)"
            }
        }
    },

    Quaternion = {
        type = "class",
        description = "Quaterion class. Constructor (x, y, z, w)",
        childs = {
            x = {type = "value"},
            y = {type = "value"},
            z = {type = "value"},
            w = {type = "value"},

            Normalize = {
                type = "method",
                description = "Normalize the Quaternion",
                args = "()",
                returns = "(number)"
            },
            Normalized = {
                type = "method",
                description = "Return the normalized Quaternion",
                args = "()",
                returns = "(Vector3)"
            },
            ToEuler = {
                type = "method",
                description = "Transform to Euler Angles XYZ",
                args = "()",
                returns = "(Vector3)"
            },
            toString = {
                type = "method",
                description = "Return a string with the values",
                args = "()",
                returns = "(string)"
            },
            SetFromAxisAngle = {
                type = "method",
                description = "Set the quaternion with the rotation of the given axis and angle",
                args = "(Vector3 axis, angle (radians))",
                returns = "()"
            },
            FromEuler = {
                type = "function",
                description = "(Static, call it from namespace)Return a quaternion with the rotation of the vector",
                args = "(x, y, z)",
                returns = "Static (Quat)"
            },
            RotateAxisAngle = {
                type = "function",
                description = "(Static, call it from namespace)Return the quaternion with the rotation of the given axis and angle",
                args = "(Vector3 axis, angle(radians))",
                returns = "Static (Quat)"
            },
            RotateX = {
                type = "function",
                description = "(Static, call it from namespace)Return the quaternion with the rotation of the X axis and angle",
                args = "(angle(radians))",
                returns = "Static (Quat)"
            },
            RotateY = {
                type = "function",
                description = "(Static, call it from namespace)Return the quaternion with the rotation of the Y axis and angle",
                args = "(angle(radians))",
                returns = "Static (Quat)"
            },
            RotateZ = {
                type = "function",
                description = "(Static, call it from namespace)Return the quaternion with the rotation of the Z axis and angle",
                args = "(angle(radians))",
                returns = "Static (Quat)"
            }
        }
    }
}
