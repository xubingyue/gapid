/*
 * Copyright (C) 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.google.gapid.service.atom;

import com.google.gapid.rpclib.binary.BinaryClass;
import com.google.gapid.rpclib.binary.BinaryObject;
import com.google.gapid.rpclib.binary.Decoder;
import com.google.gapid.rpclib.binary.Encoder;
import com.google.gapid.rpclib.binary.Namespace;
import com.google.gapid.rpclib.schema.Entity;
import com.google.gapid.rpclib.schema.Field;
import com.google.gapid.rpclib.schema.Method;
import com.google.gapid.rpclib.schema.Primitive;

import java.io.IOException;

public final class Range implements BinaryObject {
  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;
    Range range = (Range)o;
    if (myStart != range.myStart) return false;
    if (myEnd != range.myEnd) return false;
    return true;
  }

  @Override
  public int hashCode() {
    int result = (int)(myStart ^ (myStart >>> 32));
    result = 31 * result + (int)(myEnd ^ (myEnd >>> 32));
    return result;
  }

  @Override
  public String toString() {
    return "Range{start=" + myStart + ", end=" + myEnd + '}';
  }

  //<<<Start:Java.ClassBody:1>>>
  private long myStart;
  private long myEnd;

  // Constructs a default-initialized {@link Range}.
  public Range() {}


  public long getStart() {
    return myStart;
  }

  public Range setStart(long v) {
    myStart = v;
    return this;
  }

  public long getEnd() {
    return myEnd;
  }

  public Range setEnd(long v) {
    myEnd = v;
    return this;
  }

  @Override
  public BinaryClass klass() { return Klass.INSTANCE; }


  private static final Entity ENTITY = new Entity("atom", "Range", "", "");

  static {
    ENTITY.setFields(new Field[]{
      new Field("Start", new Primitive("uint64", Method.Uint64)),
      new Field("End", new Primitive("uint64", Method.Uint64)),
    });
    Namespace.register(Klass.INSTANCE);
  }
  public static void register() {}
  //<<<End:Java.ClassBody:1>>>
  public enum Klass implements BinaryClass {
    //<<<Start:Java.KlassBody:2>>>
    INSTANCE;

    @Override
    public Entity entity() { return ENTITY; }

    @Override
    public BinaryObject create() { return new Range(); }

    @Override
    public void encode(Encoder e, BinaryObject obj) throws IOException {
      Range o = (Range)obj;
      e.uint64(o.myStart);
      e.uint64(o.myEnd);
    }

    @Override
    public void decode(Decoder d, BinaryObject obj) throws IOException {
      Range o = (Range)obj;
      o.myStart = d.uint64();
      o.myEnd = d.uint64();
    }
    //<<<End:Java.KlassBody:2>>>
  }
}
